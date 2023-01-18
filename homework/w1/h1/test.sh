(time (find ./test_dir | grep -E '*.c$|*.h$' | xargs grep 'hello'> res.txt)) 2>> res.txt
