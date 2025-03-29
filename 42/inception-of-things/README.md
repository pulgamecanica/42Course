# ☁️ IoT Kubernetes Lab with Vagrant, K3s, K3d, and Ansible

![Last Commit](https://img.shields.io/github/last-commit/Jsarda42/iot)
![Repo Size](https://img.shields.io/github/repo-size/Jsarda42/iot)
![Made with Love](https://img.shields.io/badge/Made%20with-%E2%9D%A4-red?labelColor=black)
![Ansible](https://img.shields.io/badge/automation-ansible-yellow)
![Vagrant](https://img.shields.io/badge/provisioned%20by-vagrant-blue)
![K3s](https://img.shields.io/badge/Kubernetes-K3s-326CE5?logo=kubernetes)
![Python](https://img.shields.io/badge/Python-3.11-blue?logo=python)
![VirtualBox](https://img.shields.io/badge/VM-VirtualBox-183A61?logo=virtualbox)

***

This project aims to deepen your understanding of Kubernetes and DevOps practices by incrementally building environments using **Vagrant**, **K3s**, **K3d**, and **Ansible**.

You'll start with a basic virtual machine setup, then evolve into a multi-node cluster with services and automated provisioning.

---

## 📚 Project Structure

| Part        | Name                         | Description                                                                                     | Docs                               |
|-------------|------------------------------|--------------------------------------------------------------------------------------------------|------------------------------------|
| `p1`        | Manual VM Setup              | Setup multiple VMs manually with scripts and shared folders.                                     | [📄 View README](p1/README.md)     |
| `p1_ansible`| VM Setup + Ansible           | Same as `p1` but fully automated using Ansible playbooks and inventory.                         | [📄 View README](p1_ansible/README.md) |
| `p2`        | K3s Cluster + Ingress        | Build a Kubernetes cluster with `app1`, `app2`, and `app3`. Includes NGINX ingress setup.        | [📄 View README](p2/README.md)     |

---

## 🧠 Key Concepts

- **Shared `.env`** for securely passing K3s token across nodes
- **Vagrant provisioning** for launching and configuring virtual machines
- **Manual and automated cluster setup** to compare approaches
- **K3s master-agent architecture** for real-world Kubernetes experience
- **Ingress with NGINX** for routing services via domain simulation

---

## ⚙️ Requirements

- [VirtualBox](https://www.virtualbox.org/)
- [Vagrant](https://developer.hashicorp.com/vagrant)
- [Python](https://www.python.org/) (for optional scripts/venv)
- [Ansible](https://docs.ansible.com/) (used in `p1_ansible`)

---

## 🛠️ Setup Instructions

### ✅ VirtualBox Shared Folder Access

Ensure your VirtualBox configuration allows shared folder access (check your system settings under **Shared Folders → Auto-mount**).

If you're running in an environment with restricted home folders (e.g. school labs), you can override the default Vagrant home directory:

```bash
export VAGRANT_HOME=/tmp/.vagrant.d
```

---

## 🐍 Optional: Python Virtual Environment (For scripts/tools)

```bash
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

---

## 📦 Vagrant Quick Reference

### 🔧 Useful Commands

```bash
vagrant up           # Start and provision all defined VMs
vagrant halt         # Stop the VMs
vagrant destroy      # Remove all VMs
vagrant provision    # Re-run provisioning scripts
```

---

## 🚀 How It Works (Overview)

### 1. **Vagrantfile**
Defines multiple VMs with minimal resources (1 CPU, 1024 MB RAM), private networking, and shared synced folders (`/vagrant`).

### 2. **.env Token Sharing**
You define a token in `.env`:

```env
K3S_TOKEN=your-cluster-token
```

This file is shared across all VMs using Vagrant’s synced folder feature.

### 3. **K3s Installation**

- **Master Node**:
  - Reads token from `.env`
  - Runs K3s server
  - Saves `k3s.yaml` (cluster config) to shared directory
  - Copies token to `/vagrant/k3s_token`

- **Agent Node**:
  - Reads token from `/vagrant/k3s_token`
  - Connects to the master node IP
  - Joins the cluster using the token

---

## ⚙️ Ansible Setup (Part: `p1_ansible`)

### 🔹 Inventory

Defines machines and groups (e.g. `k3s_master`, `k3s_agents`). SSH keys are pulled from Vagrant.

You can inspect it:

```bash
ansible-inventory -i inventory.ini --list
```

### 🔸 Provisioning

Run from inside the VM or host:

```bash
ansible-playbook -i inventory.yml playbook.yml
```

### ✅ Install Ansible

```bash
pip install ansible
ansible --version
```

Ensure the install path is in your `PATH`.

---

## 🧪 Building the Cluster (Part: `p2`)

- Deploys:
  - `app1`: single replica
  - `app2`: 3 replicas
  - `app3`: default fallback
- Ingress controller (NGINX) exposes services via:
  - `app1.com`
  - `app2.com`
  - fallback to `app3`

---

## 🗂️ Further Reading

- [Vagrant Documentation](https://developer.hashicorp.com/vagrant/docs)
- [Provisioning with Ansible](https://developer.hashicorp.com/vagrant/docs/provisioning/ansible)
- [K3s Documentation](https://docs.k3s.io/)
- [NGINX Ingress Controller](https://kubernetes.github.io/ingress-nginx/)

---

## ✨ Bonus Tips

- Use `chmod 644` on `k3s.yaml` if you plan to access from the host.
- `KUBECONFIG=./k3s.yaml kubectl get nodes` — once the master is provisioned.
- Want more control? Add service deployments and Ingress to `ansible/playbook.yml`.

---
