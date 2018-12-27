<!-- page_number: true -->
<!-- footer: Paralelización de Disperse Construction -->

Paralelización de Disperse Construction
===

##### Programación Paralela Aplicada

###### Francisco Casas B. ( [@autopawn](https://github.com/autopawn) )

---
# Problema

![center 40%](simple_pos.png)

---
# Problema

![center 40%](simple2_lp_sol.png)


---
# Algorimo

![](code1.png)

---
# Descripción del algorimo

![center 70%](beam_search.png)


---
# Proceso de expansión

![](code2.png)

---
# Reducción simplificada

![](code3.png)

---
# Reducción con heurística-$VR$

![](code4.png)


---
# Descripción del algorimo

Parámetros significativos:

* $n$: Número de instalaciones.
* $m$: Número de clientes.
* $P$: Tamaño de *pool*.
* $V$: Rango de visión del proceso de reducción.
* $p$: Tamaño de soluciones buscado.

---
# Paralelización de la **Reducción**

* Según `callgrind` el principal cuello de botella del algoritmo es el cálculo de la **disimimilitud** entre soluciones.
* Cada una tiene complejidad $O(p^2)$ y en cada proceso de reducción deben realizarse hasta $O(V\cdot P\cdot n)$.
* Adicionalmente cada disimilitud calculada debe ser insertada en un *heap*.

---
# Reducción



---
# Paralelización de **Expansión** y **Búsqueda Local**

* Adicionalmente se paralelizaron estas dos componentes.
* Puesto que se pudo hacer sin necesidad de más concurrencia que el **join** de los threads.

---
# Desempeño

| Threads | CPU [s]    |  User [s] | Speed-up    |
| ------: |:---------: | ---------:| ----------: |
|       0 | 53.378433  | 53.408634 |       0.999 |
|       1 | 55.779545  | 55.851879 |       1.044 |
|       2 | 62.504520  | 33.169830 |       1.170 |
|       3 | 76.351929  | 31.690235 |       1.430 |
|       4 | 82.626358  | 28.790857 |       1.547 |
|       5 | 82.635376  | 28.457539 |       1.547 |
|       6 | 84.430740  | 27.015497 |       1.581 |
|       7 | 86.056892  | 26.485620 |       1.611 |
|       8 | 87.198219  | 26.476229 |       1.633 |

---
# Desempeño

| Threads | CPU [s]    |  User [s] | Speed-up    |
| ------: |:---------: | ---------:| ----------: |
|       9 | 86.720352  | 26.672344 |       1.624 |
|      10 | 87.058334  | 25.360512 |       1.630 |
|      11 | 87.778091  | 25.348858 |       1.644 |
|      12 | 88.405190  | 25.353350 |       1.655 |
|      13 | 87.763382  | 30.075008 |       1.643 | 
|      14 | 88.471825  | 32.588284 |       1.657 |
|      15 | 88.539635  | 31.982439 |       1.658 |
|      16 | 89.127106  | 29.027628 |       1.669 |

---
# Conclusiones

* El *speedup* se puede ver, sin embargo, no está muy cerca del óptimo.
* Puede ser **mayor** para problemas con $n$ o $m$ más grandes.
* El cuello de botella principal es el acceso al **heap** principal.










