# MaximumFlowAlgorithms
Uma coleção de algoritmos clássicos que resolvem o Problema do FLuxo Máximo

## Ambiente de Desenvolvimento
O projeto foi desenvolvido e testado com as seguintes versões de software, mas pode ser compatível com outras versões:

- Ubuntu 22.04.2 LTS
- C++ 14
- Python 3.10.12

  
## Compilação
Para compilar o projeto, você pode usar o utilitário `make`.
Ele compilará todos os arquivos necessários.
Certifique-se de que você está no diretório raiz do projeto antes de executar o seguinte comando:
```
make
```
Se, em algum momento, você quiser apagar os arquivos compilados para recompilar o projeto, você pode usar o seguinte comando:
```
make clean
```


## Execução
Após a compilação bem-sucedida, você pode executar o programa da seguinte maneira:
```
bin/main instances/{nome da instância} {sigla do algoritmo}
```
Substitua {nome da instância} pelo nome da instância desejada e {sigla do algoritmo} pela sigla do algoritmo que você deseja executar.


## Testes Automatizados
Para executar testes com vários algoritmos, certifique-se de que você possui o Python 3 instalado em seu sistema.
Em seguida, execute o seguinte comando:
```
python3 apps/executaTestes.py
```
Certifique-se de que tenha especificado as siglas dos algoritmos a serem executados no arquivo executaTestes.py antes de rodar o comando.


---


Isso é tudo! Agora você está pronto para compilar e executar seu projeto em C++ no Linux.
Se precisar de mais informações ou tiver alguma dúvida, sinta-se à vontade para entrar em contato conosco.
