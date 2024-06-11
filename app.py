from flask import Flask
import os,ctypes,json,codecs,subprocess
import urllib.parse

app = Flask(__name__)

@app.route("/")
def hello():
    with codecs.open('./test_data/input/parameter.json','r','utf8') as f:
        input_data = json.load(f,)
    result_data = RunPyDispatch(input_data)
    
    # 将 data 转换为 JSON 字符串
    data_json = json.dumps(result_data['geo_json'])
    
    # 对 JSON 字符串进行 URL 编码
    data_encoded = urllib.parse.quote(data_json)
    
    # 创建超链接
    ret = f"<a href='http://geojson.io/#data=data:application/json,{data_encoded}'> show result on http://geojson.io </a>"
    
    return ret


def RunPyDispatch(input_data,THIS_SO_PATH="./lib/",AB_SO_PATH="/usr/local/lib"):

    ortools_lib = ctypes.cdll.LoadLibrary(os.path.join(AB_SO_PATH,"libortools.so.9")) # libortools 通常在 usr/local/lib
    project_lib = ctypes.cdll.LoadLibrary(os.path.join(THIS_SO_PATH,"libsharing_manager.so"))        # 自己專案編譯的 .so


    # ctypes 型別 const char*(const char*)
    project_lib.PyDispatch.argtypes = [ctypes.c_char_p]
    project_lib.PyDispatch.restype = ctypes.c_char_p
    print("start Dispatch ...")
    
    bytes_data = json.dumps(input_data).encode("utf-8")

    # PyDispatch = C++ 函式名:  json_obj --> string --> json_bytes ---> C++ ---> bytes --> string ----> json_obj
    return json.loads(project_lib.PyDispatch(bytes_data).decode("utf-8"))


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

