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

k_nodes:
	@echo "@***************************@"
	@echo "@          NODES INFO       @"
	@echo "@***************************@"
	-@kubectl get nodes -o wide
	@echo "@***************************@"
	@echo "@          IP INFO          @"
	@echo "@***************************@"
	-@ip a show enp0s8
