#不同城市个数对三种串行程序性能的影响
arr=("rec" "iter1" "iter2")
for frn in ${arr[@]}
do
    echo  -e "\n*************$frn start *************\n"  
    for i in `seq 4 19`
    do  
        echo -e "=======================\n$frn program  $i cities\n======================="
        gcc -g -Wall -fopenmp -o train3_tsp_$frn train3_tsp_$frn.c &&./train3_tsp_$frn cities_$i.txt |grep time
    
    done
    echo  -e "\n*************$frn end *************\n"
done

#城市个数不同
# for i in `seq 4 10`
# do  
#     echo -e "==================\n$i cities\n=================="
#     gcc -g -Wall -fopenmp -o train3_tsp_rec train3_tsp_rec.c &&./train3_tsp_rec  cities_$i.txt

# done
# echo

#程序不同
# arr=("rec" "iter1" "iter2")
# for frn in ${arr[@]}
# do  
#     echo -e "==================\n$frn program \n=================="
#     gcc -g -Wall -fopenmp -o train3_tsp_$frn train3_tsp_$frn.c &&./train3_tsp_$frn  cities_4.txt

# done
# echo


# # 4个城市
# echo -e "==================\n4 cities\n=================="
# gcc -g -Wall -fopenmp -o train3_tsp_rec train3_tsp_rec.c &&./train3_tsp_rec  matrix_file.txt
# gcc -g -Wall -fopenmp -o train3_tsp_iter1 train3_tsp_iter1.c &&./train3_tsp_iter1 matrix_file.txt
# gcc -g -Wall -fopenmp -o train3_tsp_iter2 train3_tsp_iter1.c &&./train3_tsp_iter1 matrix_file.txt

# #５个城市
# echo -e "==================\n5 cities\n=================="
# gcc -g -Wall -fopenmp -o train3_tsp_rec train3_tsp_rec.c &&./train3_tsp_rec  cities_5.txt
# gcc -g -Wall -fopenmp -o train3_tsp_iter1 train3_tsp_iter1.c &&./train3_tsp_iter1 cities_5.txt
# gcc -g -Wall -fopenmp -o train3_tsp_iter2 train3_tsp_iter1.c &&./train3_tsp_iter1 cities_5.txt

# #６个城市
# echo -e "==================\n6 cities\n=================="
# gcc -g -Wall -fopenmp -o train3_tsp_rec train3_tsp_rec.c &&./train3_tsp_rec  cities_6.txt
# gcc -g -Wall -fopenmp -o train3_tsp_iter1 train3_tsp_iter1.c &&./train3_tsp_iter1 cities_6.txt
# gcc -g -Wall -fopenmp -o train3_tsp_iter2 train3_tsp_iter1.c &&./train3_tsp_iter1 cities_6.txt
