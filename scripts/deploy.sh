#!/bin/sh

set -ex

mkdir -p /opt/wg-alive
cd /opt/wg-alive

no_binary() {
  echo "Your system is not supported. Please compile it."
  exit 1
}

wget --compression=auto "https://somewhere/wg-alive/wg-alive.$(uname -m)" -Owg-alive || no_binary
chmod a+x wg-alive

if [ ! -f "/etc/wg-alive.conf" ]; then
  cat > /etc/wg-alive.conf <<'EOF'
# wg-alive: a WireGuard Keep-alive tool
# config file:
#   interface   gateway-ip

wg0     10.0.0.1

EOF
fi

cat > /etc/system/systemd/wg-alive.service <<'EOF'
[Unit]
Description=WireGuard Keep-alive tool
After=network-online.target

[Service]
Type=simple
DynamicUser=true
ExecStart=/opt/wg-alive/wg-alive /etc/wg-alive.conf

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload
systemctl enable wg-alive
