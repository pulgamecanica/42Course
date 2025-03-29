# 🚀 Kubernetes  Vagrant, K3s, Ansible, and Ingress

This project automates the setup of a lightweight, production-like Kubernetes environment using:

- [x] **K3s** (Lightweight Kubernetes distribution)
- [x] **Vagrant + VirtualBox** (Automated VM provisioning)
- [x] **Ansible** (Kubernetes bootstrapping + app deployment)
- [x] **NGINX Ingress Controller** (Handles external HTTP routing)
- [x] **Three containerized apps** with domain-based routing via Ingress

Perfect for learning, local testing, and fast prototyping.

---

## 📦 Project Structure

```txt

.
├── ansible
│	 ├── ansible.cfg
│	 ├── inventory.ini			# Ansible inventory pointing to Vagrant VM
│	 ├── playbook.yml			# Main provisioning logic
│	 └── roles
│	     └── common
│	         └── tasks
│	             └── main.yml
├── apps.yaml					# All app Deployments and Services
├── ingress.yaml				# Ingress definition (NGINX)
├── k3s_token
├── k3s.yaml
├── Makefile
├── README.md
├── .env
└── Vagrantfile					 # VM definition and networking
```

---

## 🧰 What’s Included

### 📁 Virtual Machine
- Debian-based VM provisioned by Vagrant
- Exposes Kubernetes via static IP: `192.168.56.110`

### ☸️ K3s (Kubernetes)
- Single-node Kubernetes (server-only)
- Installed via K3s official install script
- `traefik` explicitly disabled for custom ingress controller

### 🤖 Ansible Automation
Handles:
- K3s installation
- Token sharing
- App deployments
- NGINX Ingress installation
- Full readiness waiting (including webhook sync)

### 🌐 Ingress Controller (NGINX)
- Bare-metal NGINX ingress controller
- Exposes port `80` via `hostPort`
- Routes traffic based on domain:
  - `app1.com` → App One
  - `app2.com` → App Two
  - Default fallback → App Three

### 🐳 Apps
All apps use the official `paulbouwer/hello-kubernetes:1.10` image.

- `app-one`: 1 replica
- `app-two`: 3 replicas
- `app-three`: 1 replica (fallback route)

---

## 🌍 Access the Apps

### Test
```bash
curl -H "Host:app1.com" http://192.168.56.110     # Routes to app one
curl -H "Host:app2.com" http://192.168.56.110     # Routes to app two
curl http://192.168.56.110     # Routes to app three
```

Or open them in your browser.

---
