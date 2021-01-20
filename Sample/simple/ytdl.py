import youtube_dl
print("imported")

SAVE_PATH = 'ux0:data'

ydl_opts = {'nocheckcertificate': True, 'outtmpl':SAVE_PATH + '/%(title)s.%(ext)s'}
with youtube_dl.YoutubeDL(ydl_opts) as ydl:
    ydl.download(['https://www.youtube.com/watch?v=dQw4w9WgXcQ'])