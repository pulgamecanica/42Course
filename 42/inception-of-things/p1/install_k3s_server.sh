sudo apt-get update -y
sudo apt-get install -y curl net-tools

export $(grep -v '^#' /vagrant/.env | xargs)
# -s: silent
# -f: fail
#       (HTTP) Fail silently (no output at all) on server errors.
#       This is mostly done to enable scripts etc to  better  deal with failed  attempts. 
# -L: location 
#       (HTTP)  If the server reports that the requested page has moved to a different location
#       (indicated with a Location: header and a 3XX response code),
#       this option will make curl redo the request on the new place.
curl -sfL https://get.k3s.io | K3S_TOKEN="$TOKEN_PASS" sh -s - server --write-kubeconfig=/vagrant/k3s.yaml --flannel-iface=enp0s8 --bind-address=192.168.56.110
echo "Waiting for K3s to be ready..."
while ! sudo systemctl is-active --quiet k3s; do
    echo "K3s is not active yet. Retrying in 5 seconds..."
    sleep 5
done

sudo cp /var/lib/rancher/k3s/server/node-token /vagrant/k3s_token
sudo chmod 644 /vagrant/k3s_token

echo "K3s server setup completed successfully!"
