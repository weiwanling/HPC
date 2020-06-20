# 城市个数不同
for i in `seq 4 19`
do  
     echo -e "==================\n$i cities\n=================="
     mpicc -g -Wall -o train3_mpi_tsp_stat train3_mpi_tsp_stat.c && mpiexec -n 2 ./train3_mpi_tsp_stat  cities_$i.txt | grep time

done
echo

# 线程个数不同
for i in `seq 1 8`
do  
     echo -e "==================\n$i threads\n=================="
     mpicc -g -Wall -o train3_mpi_tsp_stat train3_mpi_tsp_stat.c && mpiexec -n $i ./train3_mpi_tsp_stat  cities_8.txt |grep time

done
echo
