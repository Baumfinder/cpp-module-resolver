run: main
	@echo ""
	@./a.out

main:
	g++ -fmodules-ts -std=c++20 main.cpp

clean:
	rm -rf gcm.cache
	rm -f a.out