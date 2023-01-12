from flask import abort

def handle_request_error(status_code:int,error_text:str) -> abort:
    abort(status_code,error_text)