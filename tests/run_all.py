import os

def change_dir():
    this_dir = os.path.dirname(os.path.realpath(__file__))
    os.chdir(this_dir)

def test_files():
    return filter(
            lambda x: x.startswith("test_"),
            os.listdir(".")
        )

def run(f):
    print "Running " + f
    os.system("g++ -std=c++11 " + f)
    os.system("./a.out")
    print ""

if __name__ == "__main__":
    change_dir()
    map(run, test_files())
