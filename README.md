# Problema do histograma em OpenMP

Compilando usando `make`:

```shell
$ make
cc -g -Wall -fopenmp    histogram.c   -o histogram
```

Ao executar o programa, deve-se inserir os valores de:

* Valores reais mínimos e máximos `<a>` e `<b>` do intervalo
* Valor inteiro da quantidade de números `<data_count>`
* Valor inteiro da quantidade de *bins* `<bin_count>`

Por exemplo, rodando usando `make run`:

```shell
$ make run
./histogram 4
Type in the values for: <a> <b> <data_count> <bin_count>: 0 5 20 5
sequence range: [0.00, 5.00]
adjusted sequence length: 20
number of bins: 5
bin interval: 1.00
[0.00, 1.00) |############# 2
[1.00, 2.00) |################################# 5
[2.00, 3.00) |######################################## 6
[3.00, 4.00) |########################## 4
[4.00, 5.00) |#################### 3
```

