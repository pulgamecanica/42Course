# ☸️ Minimal Multi-Node K3s Cluster with Vagrant

This project provisions a **lightweight multi-node Kubernetes (K3s)** cluster using **Vagrant**, with each VM running K3s installed via *custom scripts*.

> 🧪 A minimal, flexible environment for learning, experimentation, and edge-case testing.

---

## 📌 Overview

- 🖥️ **Multiple Virtual Machines**, provisioned via Vagrant
- ⚙️ **K3s installed manually** on each VM via custom scripts
- 🧵 Shared `.env` file to **predefine and control cluster token**
- 🧰 Dependencies installed using **APT (curl, net-tools, etc.)**
- 💾 **Minimal resource usage** (1 CPU, 1024MB RAM per VM)
- 🔗 Node joining is token-based and fully scriptable

---

## 📁 Project Structure

```txt
.
├── Vagrantfile             # Defines the multi-VM topology
├── scripts/
│   ├── master.sh           # K3s install script for the master node
│   └── agent.sh            # K3s install script for worker nodes
├── .env                    # Shared file storing the K3s token
```

---

## 🧠 Why This Setup?

Unlike full automation with provisioning tools (like Ansible), this project favors:

- 🧩 **Fine-grained manual control** of each VM and its installation steps
- 🛠️ Clear separation between master and agent node setup
- 🔐 Externalized token handling for flexibility and transparency
- 🧪 Focus on **learning and debugging the K3s installation process**

---


## 💡 Notes

- All machines mount the project folder via Vagrant’s shared folder (`/vagrant`)
- The K3s token is retrieved from `.env` using standard shell parsing
- You can modify resource usage in the `boxes.json` to scale the cluster up or down
- This setup is designed for **simplicity and reproducibility**, not production

---