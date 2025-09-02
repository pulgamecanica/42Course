sudo apt-get update -y
sudo apt-get install -y curl net-tools

#docker
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh
sudo usermod -aG docker $USER

# kubectl
KUBERNETES_RELEASE=$(curl -sL https://dl.k8s.io/release/stable.txt)
curl -LO "https://dl.k8s.io/release/${KUBERNETES_RELEASE}/bin/linux/amd64/kubectl"
curl -LO "https://dl.k8s.io/${KUBERNETES_RELEASE}/bin/linux/amd64/kubectl.sha256"
echo "$(cat kubectl.sha256) kubectl" | sha256sum --check || exit 1
sudo install -o root -g root -m 0755 kubectl /usr/local/bin/kubectl

# k3d
curl -s https://raw.githubusercontent.com/k3d-io/k3d/main/install.sh | bash

# Create cluster
k3d cluster create mycluster
kubectl get nodes
export KUBECONFIG="${KUBECONFIG}:${HOME}/.kube/config"

# namespaces
kubectl create namespace argocd
kubectl create namespace dev

# argocd
curl -o install.yaml https://raw.githubusercontent.com/argoproj/argo-cd/stable/manifests/install.yaml
kubectl apply -n argocd -f install.yaml

kubectl port-forward svc/argocd-server -n argocd 8080:443

# Check if ready
kubectl get pods -n argocd -o wide

# Check LoadBalancer
kubectl get svc -n argocd
kubectl get svc argocd-server -n argocd

# Get password
kubectl get secret argocd-initial-admin-secret -n argocd \
  -o jsonpath="{.data.password}" | base64 -d && echo