# TP 2 - Recuperação de Informação

A primeira parte `pass1` consiste em criar um vocabulário do dump.

Depois vai ser a parte de ordenação, `pass2`.

Por último será a de consulta pelo terminal `pass3`

## Estrutura dos arquivos

Todos os arquivos são armazenados no formato Endianness da máquina rodada.
Nos exemplos seguintes é descrita em Little Endian.

### [nome_do_arquivo].urllist

Arquivo contendo lista de URLs coletadas e separados por um '\n'.

### [nome_do_arquivo].index

Arquivo que mantém informações das posições da URL do no arquivo **[nome_do_arquivo].urllist**
e a posição do conteúdo do HTML no arquivo da coleção.

| pointer to URL | pointer to HTML content     |
| :------------- | :------------- |
| 00 00 00 00 00 00 00 00       | 10 00 00 00 00 00 00 00       |
| 0D 00 00 00 00 00 00 00       | 61 2B 05 00 00 00 00 00       |
| ...       | ...       |

Os ponteiros apontam para a posição no arquivo da coleção utilizada no TP1.

```
|||github.com|<HTML> XXX </HTML>|||youtube.com|<HTML> XXX </HTML>|||
...
showshow.com|<HTML> XXX </HTML>|||
```

O i-ésimo documento é indicado ordenadamente neste arquivo. Iniciando do 0.

### hash_table.tbl

Estrutura serve pra mapear o ID (**hash_id**) e a posição (**position**) da
string do termo com tamanho **chars_length** no arquivo **terms.dump**.

O **hash_id** é uma identificação única para cada termo.

```cpp
struct HashBlock {
  size_t hash_id;
  size_t position;
  size_t chars_length;
  size_t pointer_to_term; // Utility to next pass
  size_t freq;
};
```

| hash_id | position | chars_length | pointer_to_term | freq |
| :------- | :------- | :------- | :------- | :------- |
| 0      | 0       | 3      | X | 50 |
| 1      | 3       | 10      | X | 11 |
| 2      | 13       | 6      | X | 13 |
| ...      | ...       | ...      | ... | ... |
| n | position(n) | size(n) | X | 5 |

### terms.dump

Contém strings concatenadas para consulta utilizando parâmetros do arquivo **hash_table.tbl**.

```
<term1><term2><term3>...<termN>
```

### coleção de termos na pasta terms

Esta pasta serve para armazenar a frequência e posição do termo no HTML do
arquivo da coleção (não a posição na coleção).

Ele segue a estrutura:

```cpp
struct {
  size_t hash_id;
  size_t document_id;
  size_t position;
};
```

| hash_id        | document_id       | position           |
| :------------- | :------------- | :------------- |
| 00 00 00 00 00 00 00 00 | 00 00 00 00 00 00 00 00 | B1 09 00 00 00 00 00 00 |
| 01 00 00 00 00 00 00 00 | 00 00 00 00 00 00 00 00 | B6 09 00 00 00 00 00 00 |
| ... | ... | ... |

Cada termo tem um ID único (**hash_id**), assim, para evitar criar um arquivo
e evitar paddings dos arquivos no sistema operacional
para cada é dividido em blocos na pasta termos.

`(hash_id / MANY_ON_DAT_BLOCK)`

**MANY_ON_DAT_BLOCK**
`MANY_ON_DAT_BLOCK` é definido no **term_manage.h**.
Os primeiros `MANY_ON_DAT_BLOCK` termos são armazenados no arquivo **0**.
Os próximos `MANY_ON_DAT_BLOCK` no **1** e assim por diante.

Observe que como a varredura segue sequencialmente nos documentos, o document_id
será ordenado nestes arquivos.

## XXX

Assim, seguindo a ideia anterior, basta agora ordernar mas preocupando em utilizar
uma ordenação estável. E como pedido terá que ser uma ordenação externa.
Para testes será uma ordenação interna.
