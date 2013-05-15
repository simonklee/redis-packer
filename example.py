import redis
import redis_packer

from redis_packer import pack_command
v = pack_command('1', 2.2, u'3', 1)
print v

class MyConnection(redis.Connection):
    pack_command = redis_packer.pack_command

pool = redis.ConnectionPool(connection_class=MyConnection)

conn = redis.StrictRedis(connection_pool=pool)
print conn.exists('foo')
