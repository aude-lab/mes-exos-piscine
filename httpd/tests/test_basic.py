import pytest
import requests
import socket
import subprocess as sp
import time
import signal
import os

HTTPD_BIN = "./httpd"
PID_FILE = "/tmp/httpd_test.pid"
PORT = "8888"
IP = "127.0.0.1"
ROOT_DIR = "/tmp/test_httpd"
SERVER_NAME = "test_server"
BASE_URL = f"http://{IP}:{PORT}"

@pytest.fixture(scope="function")

def httpd_server():
    os.makedirs(ROOT_DIR, exist_ok=True)
    
    with open(f"{ROOT_DIR}/index.html", "w") as f:
        f.write("<h1>Hello World!</h1>")
    
    with open(f"{ROOT_DIR}/test.html", "w") as f:
        f.write("<h1>Test Page</h1>")
    
    process = sp.Popen([
        HTTPD_BIN,
        "--pid_file", PID_FILE,
        "--port", PORT,
        "--ip", IP,
        "--root_dir", ROOT_DIR,
        "--server_name", SERVER_NAME,
        "--log", "true"
    ], stdout=sp.PIPE, stderr=sp.PIPE)
    
    time.sleep(0.5)
    
    yield process
    
    try:
        process.send_signal(signal.SIGINT)
        process.wait(timeout=2)
    except:
        process.kill()
    
    if os.path.exists(PID_FILE):
        os.remove(PID_FILE)


def test_get_request(httpd_server):
    response = requests.get(f"{BASE_URL}/index.html", timeout=2)
    
    assert response.status_code == 200
    assert "Hello World!" in response.text
    assert "text/html" in response.headers.get("Content-Type", "")
    assert response.headers.get("Connection") == "close"


def test_head_request(httpd_server):
    response = requests.head(f"{BASE_URL}/index.html", timeout=2)
    
    assert response.status_code == 200
    assert len(response.content) == 0  
    assert "Content-Length" in response.headers
    assert response.headers.get("Connection") == "close"

def test_403_forbidden(httpd_server):
    protected_file = f"{ROOT_DIR}/protected.html"

    with open(protected_file, "w") as f:
        f.write("<h1>Protected Content</h1>")

    os.chmod(protected_file, 0o000) 

    try:
        response = requests.get(f"{BASE_URL}/protected.html", timeout=2)

        assert response.status_code == 403
        assert "403 Forbidden" in response.text
        assert "Forbidden" in response.text

    finally:
        os.chmod(protected_file, 0o644)
        os.remove(protected_file)

def test_404_not_found(httpd_server):
    response = requests.get(f"{BASE_URL}/notfound.html", timeout=2)
    
    assert response.status_code == 404
    assert "404 Not Found" in response.text


def test_405_method_not_allowed(httpd_server):
    response = requests.post(f"{BASE_URL}/index.html", timeout=2)
    
    assert response.status_code == 405
    assert "405 Method Not Allowed" in response.text


def test_default_file(httpd_server):
    response = requests.get(f"{BASE_URL}/", timeout=2)
    
    assert response.status_code == 200
    assert "Hello World!" in response.text


def test_malformed_request(httpd_server):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((IP, int(PORT)))
    
    sock.sendall(b"GET\r\n\r\n")
    response = sock.recv(1024).decode()
    sock.close()
    
    assert "400 Bad Request" in response


def test_http_version_not_supported(httpd_server):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((IP, int(PORT)))
    
    sock.sendall(b"GET /index.html HTTP/2.0\r\nHost: test\r\n\r\n")
    response = sock.recv(1024).decode()
    sock.close()
    
    assert "505 HTTP Version Not Supported" in response


def test_multiple_requests(httpd_server):
    for i in range(5):
        response = requests.get(f"{BASE_URL}/index.html", timeout=2)
        assert response.status_code == 200
        assert "Hello World!" in response.text
