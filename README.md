# RTRES
<p>
    Programa C UNIX style com interpretador simples de arquivo
    e Manual Man em groff.
</p>

```
    $ rtres -h          #Imprime ajuda
    $ man ./rtres.1     #exibe manual man em groff


    # REGRA DE TRES DIRETA
    # se um carro leva uma hora para
    # andar 60km então levará X horas para
    # andar 250km

    $ rtres -d 60 3 250
    12.000000

    # REGRA DE TRES INVERSA
    # se 12 pedreiros concluem a obra em 5dias
    # então X pedreiros concluem em 3dias

    $ rtres -i 12 5 3
    20.000000

    #interpretar o arquivo escrito
    #da seguinte forma
    # 60:1:120:i
    # 50:2:121:i
    # 10:4:119:i
    # 20:3:115:i
    # e retornar os respectivos resultados.

    $ rtres -f calculos
    60:1:120:
     2.000000
    50:2:121:
     4.000000
    10:4:119:
     47.000000
    20:3:115:
     17.000000


```
