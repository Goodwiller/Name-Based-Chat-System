
# make file for the dns server and client

client:
	@echo "Compiling the Client..."
	g++ -o client -pthread client.cpp client_class.cpp


dns:
	@echo "Compiling the dns Server..."
	g++ -o dns dns.cpp dns_class.cpp
	./dns


clean:
	@echo "Removing all object files..."
	rm client dns
