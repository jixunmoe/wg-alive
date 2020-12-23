#!/bin/sh

set -ex

mkdir -p /opt/wg-alive

no_binary() {
  echo "Your system is not supported. Please compile it."
  exit 1
}

VER=$(wget -q -O- https://api.github.com/repos/jixunmoe/wg-alive/releases/latest | jq -r '.tag_name')
URL="https://github.com/jixunmoe/wg-alive/releases/download/${VER}/wg-alive.$(uname -m)"
wget --compression=auto "$URL" -O /opt/wg-alive/wg-alive || no_binary
chmod a+x /opt/wg-alive/wg-alive

if [ ! -f "/etc/wg-alive.conf" ]; then
  cat > /etc/wg-alive.conf <<'EOF'
# wg-alive: a WireGuard Keep-alive tool
# config file:
#   interface   gateway-ip

wg0     10.0.0.1

EOF
fi

cat > /etc/systemd/system/wg-alive.service <<'EOF'
[Unit]
Description=WireGuard Keep-alive tool
After=network-online.target

[Service]
Type=simple
ExecStart=/opt/wg-alive/wg-alive /etc/wg-alive.conf

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload
systemctl enable wg-alive

editor /etc/wg-alive.conf
