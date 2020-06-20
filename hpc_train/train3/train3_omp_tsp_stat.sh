# 城市个数不同
for i in `seq 4 19`
do  
     echo -e "==================\n$i cities\n=================="
     gcc -g -Wall -fopenmp -o train3_omp_tsp_stat train3_omp_tsp_stat.c &&./train3_omp_tsp_stat 2 cities_$i.txt | grep time

done
echo

# 线程个数不同
for i in `seq 1 8`
do  
     echo -e "==================\n$i threads\n=================="
     gcc -g -Wall -fopenmp -o train3_omp_tsp_stat train3_omp_tsp_stat.c &&./train3_omp_tsp_stat $i cities_8.txt |grep time

done
echo
