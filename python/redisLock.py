import sys
import uuid
import redis
import time
import threading

class myRedisLock(object):
    def __init__(self, redis_host, redis_port):
        self.redis = redis.Redis(connection_pool = redis.ConnectionPool(
                    host = redis_host,
                    port = redis_port,
                    db = 0))
        self.mutux_key = "myredislock"
        self.mutux_value = str(threading.current_thread().ident) + str(uuid.uuid4())
        self.is_update_lock_time = False
    
    def _updateLockTime(self):
        deamon_thread = threading.Thread(target=self.start_update, name="update_lock_time_thread")
        deamon_thread.start()
    
    def start_update(self):
        while self.is_update_lock_time:
            print("update lock time")
            cmd = '''if redis.call("get", KEYS[1]) == ARGV[1] then
                    return redis.call("expire", KEYS[1], ARGV[2])
                    else return 0 end'''
            self.redis.eval(cmd, 1, self.mutux_key, self.mutux_value, 10)
            time.sleep(5)

    def lock(self):
        while True:
            ret = self.redis.set(self.mutux_key, self.mutux_value, nx=True, ex=10)
            #print("ret: %s" % ret)
            if ret and int(ret) > 0:
                print("%s 拿到了锁" % threading.current_thread().name)
                self.is_update_lock_time = True
                self._updateLockTime()
                break
            else:
                time.sleep(1)
            
    def unlock(self):
        cmd = '''
            if redis.call("get", KEYS[1]) == ARGV[1] then
            return redis.call("del", KEYS[1])
            else return 0 end
        '''
        self.redis.eval(cmd, 1, self.mutux_key, self.mutux_value)
        self.is_update_lock_time = False

potatos = 10  
redis_host = "127.0.0.1"
redis_port = 7999 
def main():  
    for i in range(15):
        th = threading.Thread(target=buy_potatos, name="thread_%s"%str(i))
        th.start()

def buy_potatos():
    global potatos
    mylock = myRedisLock(redis_host, redis_port)
    mylock.lock()
    if potatos > 0:
        print("%s buy 1 potato"%threading.current_thread().name)
        potatos -= 1
    else:
        print("%s buy fail, potato already out" % threading.current_thread().name)
    time.sleep(15)
    mylock.unlock()

if __name__ == "__main__":
    main()