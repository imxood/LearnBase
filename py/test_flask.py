from flask import Flask
from flask import request

app = Flask(__name__)

@app.route('/')
def hello1_world():
    return 'Hello World!'

@app.route('/test', methods=['GET', 'POST'])
def hello2_world():
    if request.method == 'POST':
        username = request.form.get('username', 'null')
        password = request.form.get('password', 'null')
    else:
        username = request.values['username']
        password = request.values['password']
    return 'username:%s,password:%s' % (username, password)


if __name__ == '__main__':
    app.run(debug=True)