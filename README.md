# 2026_inhatc
2026 년 3학년 1학기 (A,B,C반)


## 라즈베리파이 초기 설정
```
sudo apt update
sudo apt upgrade
```

  - 한글깨짐
```
sudo apt-get install fonts-unfonts-core -y
sudo apt-get install ibus ibus-hangul -y
sudo reboot
```

## InfluxDB 설치
  - 기존 키 삭제
```
  sudo rm /usr/share/keyrings/influxdata-archive_compat.gpg
  sudo rm /etc/apt/sources.list.d/influxdata.list
```
  - InfluxDB download GPG key using wget
```
curl -fsSL https://repos.influxdata.com/influxdata-archive.key \
| sudo gpg --dearmor -o /usr/share/keyrings/influxdata-archive.gpg
echo "deb [signed-by=/usr/share/keyrings/influxdata-archive.gpg] \
https://repos.influxdata.com/debian stable main" \
| sudo tee /etc/apt/sources.list.d/influxdata.list
```
  - Packages are up to date && install Influxdb
```
 sudo apt-get update && sudo apt-get install influxdb -y

```
  - InfluxDB as a background service on startup
```
sudo service influxdb start
```
  - InfluxDB is status (service)
```
sudo service influxdb status
```
  
## InfluxDB 데이터베이스 만들기

```
$ influx

>create database <데이터베이스이름>
확인 : show databases 
```

# Grafana Installation

## 1. Install the prerequisite packages
```
sudo apt-get install -y apt-transport-https software-properties-common wget
```

## 2. Import the GPG key:
```
sudo mkdir -p /etc/apt/keyrings/
wget -q -O - https://apt.grafana.com/gpg.key | gpg --dearmor | sudo tee /etc/apt/keyrings/grafana.gpg > /dev/null
```

## 3. To add a repository for stable releases, run the following command:
```
echo "deb [signed-by=/etc/apt/keyrings/grafana.gpg] https://apt.grafana.com stable main" | sudo tee -a /etc/apt/sources.list.d/grafana.list
```

## 4. Run the following command to update the list of available packages:
```
sudo apt-get update && sudo apt-get install grafana -y

```
## 5. Run the following command to server start
```
sudo systemctl start grafana-server
```
## influxdb import with python
```
pip install influxdb
```
  - hint: See PEP 668 for the detailed specification.
```
  sudo rm /usr/lib/python3.11/EXTERNALLY-MANAGED
```

# TelegramBot
```
  pip install python-telegram-bot --upgrade
  git clone https://github.com/python-telegram-bot/python-telegram-bot
  pip install python-telegram-bot[job-queue] --pre
```
## TelegramBot Docs
  - https://docs.python-telegram-bot.org/en/stable/telegram.ext.jobqueue.html

## PI 카메라 연결
  - Legacy Camera disable
```
  libcamera-hello -t 0
```
  - Python Lib 설치
```
  pip install picamera2
```
  - Error
```
libEGL warning : DRI2: failed to authenticate
Made X/EGL preview window
[1773] INFO Camera camera_manager.cpp:297 libcamera v0.0.5+83-bde9b04f
ERROR: *** no cameras available ***
```
  - 참고
```
  https://github.com/raspberrypi/picamera2/blob/main/examples/capture_png.py
```

