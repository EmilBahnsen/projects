from http.server import HTTPServer, SimpleHTTPRequestHandler

# Web-server
with HTTPServer(('', 80), SimpleHTTPRequestHandler) as httpd:
    httpd.serve_forever()
