all:
	gcc -shared -fPIC -o ddmon.so ddmon.c -ldl
	gcc -shared -fPIC -o ddchck.so ddchck.c -ldl
	gcc -o example1 -g example1.c -lpthread
	gcc -o example2 -g example2.c -lpthread
	gcc -o dinning -g dinning.c -lpthread
	gcc -o abba -g abba.c -lpthread
clean:
	rm -rf ddmon.so ddchck.so example1 example2 dinning abba
