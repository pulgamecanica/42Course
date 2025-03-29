up:
	@vagrant up

destroy:
	@vagrant destroy -f
	@rm -rf .vagrant

check:
	ansible-inventory -i ansible/inventory.ini  --list

ping:
	-@ansible JsardaS -m ping -i ansible/inventory.ini
	-@ansible JcervoniSW -m ping -i ansible/inventory.ini


re: destroy up


# Rules for managed nodes

k_ingress:
	@echo "@***************************@"
	@echo "@       INGRESS INFO        @"
	@echo "@***************************@"
	-kubectl get ingress -o wide
	@echo "@***************************@"
	@echo "@      INGRES DESCRIBE      @"
	@echo "@***************************@"
	kubectl describe ingress app-ingress
	@echo "@***************************@"
	@echo "@         INGRES SVC        @"
	@echo "@***************************@"
	kubectl get svc -n ingress-nginx
	
k_deployments:
	@echo "@***************************@"
	@echo "@      DEPLOYMENTS INFO     @"
	@echo "@***************************@"
	@kubectl get deployments -o wide

k_replicas:
	@echo "@***************************@"
	@echo "@       REPLICAS INFO       @"
	@echo "@***************************@"
	-kubectl get rs
	@echo "@***************************@"
	@echo "@    REPLICAS LABELS INFO   @"
	@echo "@***************************@"
	@kubectl get pods --show-labels

k_services:
	@echo "@***************************@"
	@echo "@       SERVICES INFO       @"
	@echo "@***************************@"
	kubectl get services -o wide

k_nodes:
	@echo "@***************************@"
	@echo "@        NODES INFO         @"
	@echo "@***************************@"
	-kubectl get nodes -o wide
	@echo "@***************************@"
	@echo "@          IP INFO          @"
	@echo "@***************************@"
	-@ip a show enp0s8

k_endpoints:
	@echo "@***************************@"
	@echo "@       ENDPOINTS INFO      @"
	@echo "@***************************@"
	kubectl get endpoints

k_all: k_deployments k_replicas k_ingress k_services k_nodes