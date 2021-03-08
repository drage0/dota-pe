import urllib.request
import json

#
# to grab your own personl key, visit https://steamcommunity.com/dev
#
url_herolist = "http://api.steampowered.com/IEconDOTA2_570/GetHeroes/v1?language=en&key= Y O U R   K E Y   H E R E"

herlist_content = urllib.request.urlopen(url_herolist).read();
herolist_data   = json.loads(herlist_content)["result"]
herocount       = herolist_data["count"]
herolist        = herolist_data["heroes"]

print(herocount)
for hero in herolist:
    print("\""+hero["localized_name"]+"\", "+str(hero["id"]))


