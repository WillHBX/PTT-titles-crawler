import requests
from bs4 import BeautifulSoup
import time
from random import randint 

def lineNotifyMessage(token, msg):
    headers = {
          "Authorization": "Bearer " + token, 
          "Content-Type" : "application/x-www-form-urlencoded"
      }
    payload = {'message': msg}
    r = requests.post("https://notify-api.line.me/api/notify", headers = headers, params = payload)
    return r.status_code

token = 'my_token'
message = '有票RRR'
errorMsg= '出事啦QAQ'
URL = "https://www.ptt.cc/bbs/Drama-Ticket/index.html"
lastOne = ""

response = requests.get(URL)
soup = BeautifulSoup(response.text, 'html.parser')
titles = soup.find_all('div', {'class': 'title'})
lastOne = titles[-5] #last 4 titles are announcements by the moderator
time.sleep(1)

while True:
    try:
        response = requests.get(URL)
        if(response.status_code==200):
            soup = BeautifulSoup(response.text, 'html.parser')
 
            titles = soup.find_all('div', {'class': 'title'})
    
            for title in titles[-5::-1]:       
                if title == lastOne:
                    break
       
                if '五月天' in title.text and '售' in title.text:
                    lineNotifyMessage(token,message+title.text)
                    print(title.text)
            
            lastOne = titles[-5]

            waiting_times = 15+randint(0,40)
            print(waiting_times)
            time.sleep(waiting_times)
    except: 
        lineNotifyMessage(token,errorMsg)
