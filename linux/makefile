main:   main.o
        g++ -o main $^
        
main.o: main.cpp
        g++ -c $^

run:
        chmod 100 main
        rm *.o
        @echo "----------------------------------------------"
        ./main
