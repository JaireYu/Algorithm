import random
for i in range(1, 100):
    f = open("D:\\GithubLocalRepo\\Algorithm\\Nearest_Vertex\\Nearest_Vertex\\Debug\\test{}.txt".format(str(i)), "w")
    for j in range(0, 200):
        f.write(str(random.random()*100))
        f.write(" ")
        f.write(str(random.random()*100))
        f.write("\n")
