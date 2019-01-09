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
# Algoritmo

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

Se probarán con dos instancias de SPLP de [UrlLib](http://resources.mpi-inf.mpg.de/departments/d1/projects/benchmarks/UflLib/index.html):
`Euclid` y `GapA`.

---
# Profiling (problema Euclid)

![center](small_problem.png)

---
# Profiling (problema GapA)

![center](big_problem.png)

---
# Paralelización de la **Reducción**

* Según `perf` el principal cuello de botella del algoritmo es el cálculo de la **disimimilitud** entre soluciones.
* Cada una tiene complejidad $O(p^2)$ y en cada proceso de reducción deben realizarse hasta $O(V\cdot P\cdot n)$.
* Adicionalmente cada disimilitud calculada debe ser insertada en un *heap*.

---
# Reducción
![center 80%](reduccion_00.png)

---
# Reducción
![center 80%](reduccion_01.png)

---
# Reducción
![center 80%](reduccion_02.png)

---
# Paralelización de la Reducción

* Se utilizó un mutex para acceder al *heap*.
* Cada thread computa una ráfaga de $V$ disimilitudes y luego las agrega al heap.
* En las reposiciones se deben agregar $V$ disimilitudes, cada thread agrega $V/n$, usando `trylock`, si no puede obtener el lock, las almacena en un buffer.
* De esta manera siempre está ocupado el *heap*.


---
# Paralelización de la Reducción

```c
void *reduce_thread_execution(void *arg){
  // ...
  for(int i=args->thread_id;i<args->n_sols;i+=THREADS){
    for(int j=1;j<=args->vision_range;j++){
      if(i+j>=args->n_sols) break;
      // -> compute dissimilitude between i and i+j
    }
    pthread_mutex_lock(args->heap_mutex);
    // -> save dissimilitudes in heap
    pthread_mutex_unlock(args->heap_mutex);
    // -> delete dissimilitudes here
  }
  free(pairs);
```

---

```c
while(1){
  sem_post(args->complete_sem);
  // ---@> Main thread works here, read terminated.
  sem_wait(args->thread_sem);
  if(terminated) break;
  // Create new pairs
  for(int i=args->thread_id;i<args->vision_range;
      i+=THREADS){
    // -> compute dissim between prev[vr-1-i] and next[i]
    // -> dissim to buffer
    if(pthread_mutex_trylock(args->heap_mutex)==0){
      // -> add dissims in buffer
      pthread_mutex_unlock(args->heap_mutex);
    }
  }
  // if buffer not empty, wait for mutex
  if(pair_buffer_len>0){
    pthread_mutex_lock(args->heap_mutex);
    // -> add dissims in buffer
    pthread_mutex_unlock(args->heap_mutex);
    pair_buffer_len = 0;
  }
  return NULL;
}
```

---
# Paralelización de **Expansión** y **Búsqueda Local**

* Adicionalmente se paralelizaron estas dos componentes.
* Puesto que se pudo hacer sin necesidad de más concurrencia que el **join** de los threads.


---
# Speedup Euclid

![150% center](euclid.png)

<!--
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
-->

---
# Speedup Euclid

![center](euclid_plot.png)

---
# Speedup GapA

![150% center](gapa.png)
<!---
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
--->

---
# Speedup GapA

![center](gapa_plot.png)


---
# Conclusiones

* Se puede ver un *speedup* cercano a 2.0 cuando el óptimo es 4.0, lo que puede deberse a las tareas que realiza el *thread* principal.
* El *speed-up* puede ser **mayor** para problemas en que el tamaño de las soluciones y $V$ son mayores, puesto que el tiempo de calcular aumenta en relación con es mayor que el acceso al *heap* y otras tareas.
* La implementación con `trylock` permitió pasar de *speedup* ${\approx} 1.6$ a ${\approx} 2.0$.

<!-- * El cuello de botella principal es el acceso al **heap** principal. -->