sudo apt-get update -y
sudo apt-get install -y curl net-tools

K3S_SERVER="https://192.168.56.110:6443"

echo "Waiting for K3s token..."
while [ ! -f /vagrant/k3s_token ]; do
    echo "K3s token not found, retrying in 5 seconds..."
    sleep 5
done

K3S_TOKEN=$(cat /vagrant/k3s_token)
echo "K3s token retrieved: $K3S_TOKEN"

echo "Installing K3s agent..."
# -s: silent
# -f: fail
#       (HTTP) Fail silently (no output at all) on server errors.
#       This is mostly done to enable scripts etc to  better  deal with failed  attempts. 
# -L: location 
#       (HTTP)  If the server reports that the requested page has moved to a different location
#       (indicated with a Location: header and a 3XX response code),
#       this option will make curl redo the request on the new place.
curl -sfL https://get.k3s.io | K3S_URL=$K3S_SERVER sh -s - agent --token $K3S_TOKEN --node-ip 192.168.56.111
echo "K3s agent installation complete!"
sudo mkdir -p /etc/rancher/k3s
sudo cp /vagrant/k3s.yaml /etc/rancher/k3s