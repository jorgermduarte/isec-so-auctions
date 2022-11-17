# Requisitos:

- [x] Planear e definir as estruturas de dados responsáveis por gerir as definições de funcionamento no frontend e
no backend. 
- [x] Definir os header files com constantes simbólicas e declarações associadas às estruturas de dados.

## Frontend:

- [x] implementar a parte da leitura de comandos e respetiva validação. 
  - [x] Todos os comandos devem ter a sua sintaxe validada. 
    - [x] Os comandos não farão ainda nada, mas será reconhecido como válido ou inválido, incluindo parâmetros. 
  - [x] Implementar a recepção das credenciais do utilizador.
  
## Backend:
  
  - [x] Implementar a leitura de comandos do administrador, validando a sintaxe de todos. Os comandos não farão
  ainda nada, mas será reconhecido como válido ou inválido, incluindo parâmetros.
  - [x] Implementar a parte de lançamento dos promotores e recepção das suas frases. Nesta meta, apenas será
    lançado o primeiro promotor.
    
  - [x] Leitura e atualização dos utilizadores através da biblioteca fornecida para esse efeito. De cada vez que a
    funcionalidade é testada, os utilizadores perdem 1 no saldo ficando essa alteração gravada. (test_lib method)

  - [x] Leitura do ficheiro dos itens à venda. Os dados serão lidos e interpretados. Não se trata de apenas ler e
      imprimir um ficheiro de texto - os valores inteiros serão armazenados e mostrados como inteiros.

  - [x] A verificação da funcionalidade dos comandos / execução do promotor / utilizadores / itens será feira por
      opção do utilizador: a função main irá perguntar algo como “deseja testar que funcionalidade?”.


## Frontend / Backend: 

  - [x] makefile que possua os targets de compilação “all” (compilação de todos os programas), “frontend” (compilação
  do programa frontend), “backend” (compilação do programa backend) e “clean” (eliminação de todos os
  ficheiros temporários de apoio à compilação e dos executáveis).

## Data de entrega: Domingo, 20 de Novembro, 2022.

Na meta 1 deverá ser entregue um breve documento (pdf, incluído no ficheiro zip) com duas páginas descrevendo os
pormenores da implementação e principais opções tomadas. A capa de título não conta como página.