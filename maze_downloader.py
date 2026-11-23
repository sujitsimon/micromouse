import requests
from datetime import datetime
import random
import os
from tqdm import tqdm

maze_download_folder = "maze_download_32x32"
max_files_to_gen = 3000
proxy = { "http": "http://127.0.0.1:3128",
          "https": "http://127.0.0.1:3128" }

__VIEWSTATE_COUNT = 776
__EVENTVALIDATION_COUNT = 752

def download_maze(datetime_obj):
    validation='V4bCsq6J8E7PXe/KfgtAxivLnwoU5MzWuaIBmrRlHQ+Dc9yGFOp0dkS3NjhZT2Y1'
    state='V4bCq621J8E7eXP/KfgtAxivLnwo5UMWuaBIRrmlQH+=D9cyGpOF0dk3NSjhZTsYz'
    post_headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/115.0',
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8',
        'Accept-Language': 'en-US',
        'Referer': 'https://www.mazegenerator.net/',
        'Content-Type': 'application/x-www-form-urlencoded',
        'Origin': 'https://www.mazegenerator.net',
        'DNT': '1',
        'Cookie': f'_ga_1KVLZKDHCJ=GS1.1.1712812192.2.1.{int(datetime_obj.timestamp()) + 20}.0.0.0; _ga=GA1.1.172272587.1712038650; ASP.NET_SessionId=ymegecy5ubhstfg24323cdz1',
        'Upgrade-Insecure-Requests': '1',
        'Sec-Fetch-Dest': 'document',
        'Sec-Fetch-Mode': 'navigate',
        'Sec-Fetch-Site': 'same-origin',
        'Sec-Fetch-User': '?1',
        'TE': 'trailers',
    }

    get_headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/115.0',
        'Accept': 'image/avif,image/webp,*/*',
        'Accept-Language': 'en-US',
        'Referer': 'https://www.mazegenerator.net/',
        'DNT': '1',
        'Cookie': f'_ga_1KVLZKDHCJ=GS1.1.1712812192.2.1.{int(datetime_obj.timestamp()) + 20}.0.0.0; _ga=GA1.1.172272587.1712038650; ASP.NET_SessionId=ymegecy5ubhstfg24323cdz1',
        'Sec-Fetch-Dest': 'image',
        'Sec-Fetch-Mode': 'no-cors',
        'Sec-Fetch-Site': 'same-origin',
        'TE': 'trailers',
    }
    # print(headers['Cookie'])
    timestamp_c = datetime_obj.strftime("%Y%m%d%H%M%S")
    # print(f'https://www.mazegenerator.net/ImageGenerator.ashx?Tag={timestamp_c}&MazeType=1&Solution=1')
    temp_view_state = ""
    temp_event_validation = ""
    while(len(temp_view_state)< __VIEWSTATE_COUNT):
        temp_view_state += state[random.randint(0,len(state)-1)]
    while(len(temp_event_validation)< __EVENTVALIDATION_COUNT):
        temp_event_validation += validation[random.randint(0,len(validation)-1)]
    temp_event_validation = "fTkDu2UnIxo8jPaJc1Cz0O1Zq/wC9A/q6Y9yFOSct+xriz9VGOX4ySpVdzBoyC6qbeXWJkyhtjVW+/mPJKfoWuYE4qjipG+ocRDeDWgW6cZLsbdKJA6tw3tzmHuWq14CE09OZENnhm7YAwW/HQRdTiDBcokiApM7qOrbJfHSqJ7EUhDFU29lW8cO0MuVdFtgRPCqk6LGzy8h9prhIEWpHwhYAADrSzEPnMU4316341eL2Lyp47okOIPadJfgQ+jTkX4VBnkzF1VRZXycKN4aLHJ8OJsynq1pQV4WrJ1N4vdMwfkruvzja6kcs4zHHAjfoGBGDINKTCMSIJOUlDYdAJKYb9C/tB0IMxS92Oqfbnciwh4umNiXk947eYhg0Vr0A91GsmM1k1uRJQsCihUHoAhF97Hd05FPE0spZ8Gj69Eq9zuyXJ7Nc+bT8r0Bxq35F28hpjbfuIZzx6VPX4428TLcb9/vA9wvx46Q5l16vGcbD2L3oTdcX+vstKAPKnKGhegyjDThUQH03f22kKgcIWFdfj8GHub3yRyKwojzDZpxjXxadE7ct4dOkYdVKsfFYwae+9tvKZBeuxMANo6ZN6Wk4isp9g0tJyTtf/IXUkopsP5X+3OhymWndg4xVgdZ0fN9LKVctCmL4ekC0xsCQA6uq6HF4aoyIMRtPu1MSomgasmnY7RQp+BfBMgT8E7RHk6bQAlTz/0GqVbXqpUpywR5p05tjsU1I3MR6GGQ02NoNtJp"
    temp_view_state = "TGEeXKzQ8fh4BKtnZ/VY58hWdIjyJuwLKXxTL4dr6kjuChl3yPLjnZAuJUVz8rBUIOnVA1PYsidVnB6TdJAAh9sQzOkD9LNSqalPBOeZbXZS6C8hDivRuEg3XjZImGyoG1ikScyEtK9RBanZVoMA5nNYh+A+Q3LG2qWQTbCQdpnbjdNyNfDQVBBoolbu1+Y5TiZW76LQX2xxxluj+eiDKWeuGRo26ONm9RB+kJcHYtC8Be7piZUo3kWY4JDf/dUn37l7MUOSji9KckgwlmI2cnFVpsSyUEnJtmb6CnoWAqjHpWsK0cbK0IQYvPjHc+OzGxOA8wc4zu5KtLlP89Ll6tM7coZmdS8+YR7m1Gwf5kT8P3WwOADVYfuEOXwY9KeSY3cl89sRlL2CWh8p7y3/kkQExCvHDQb08suEjPMu63uOC0qf/fcx5sitpH/x9t79kgKOFxYT7TS4u9VjhkP47Of3OIT6uMH9ckMKAiRfBOw9JE1jvGCxEnRnhqqs7hzQkvNuuZ5Hxxy4gOqH5TMxVms17ZPUD7v+YtidqLcLdh1GS3B+c5pcTbPc3uThExJEX6uHE5PDRj22hNtQdplK2EAyFkK3eOHmWxICqcbt8hIuCmlyaUIdXM4ftczbcfKkSCXGPc+gJpHEhSuSz5HJ2QFi7BxUFKtE17o4Vt0AKSIU+OqNTOmktzoA4Gy/IG4UZUFweld3SidbNOMlnPt7JeA0mOTMR7XqmS/s+wLzd7LHAQurVN2IJ809QVcOqY2/ehpStg=="
    payload = {
        '__EVENTTARGET': '',
        '__EVENTARGUMENT': '',
        '__LASTFOCUS': '',
        '__VIEWSTATE': temp_view_state,
        '__VIEWSTATEGENERATOR': 'CA0B0334',
        '__EVENTVALIDATION': temp_event_validation,
        'ShapeDropDownList': '1',
        'S1TesselationDropDownList': '1',
        'S1WidthTextBox': '32',
        'S1HeightTextBox': '32',
        'S1InnerWidthTextBox': '2',
        'S1InnerHeightTextBox': '2',
        'S1StartsAtDropDownList': '1',
        'AlgorithmParameter1TextBox': '50',
        'AlgorithmParameter2TextBox': '100',
        'GenerateButton': 'Generate new',
        'ShowSolutionCheckBox': 'on',
        'FileFormatSelectorList': '1',
    }
    response = requests.post('https://www.mazegenerator.net/', headers=post_headers, data=payload, proxies=proxy)
    if response.status_code == 200:
        pass
        #print("Post Success")
    else:
        print("Post Failed with: ",response.status_code)
        return
    response = requests.get(f'https://www.mazegenerator.net/ImageGenerator.ashx?Tag={timestamp_c}&MazeType=1&Solution=1', headers=get_headers, proxies=proxy)
    
    # Check if the request was successful (status code 200)
    if response.status_code == 200:
        with open(os.path.join(os.getcwd(), maze_download_folder, f'{timestamp_c}.svg'), 'wb') as f:
            f.write(response.content)
        #print("Image saved successfully.")
    else:
        print(f"Failed to fetch image. Status code: {response.status_code}")

if __name__ == "__main__":
    for i in tqdm(range(max_files_to_gen)):
        download_maze(datetime.now())