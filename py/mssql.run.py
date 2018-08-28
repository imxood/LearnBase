# coding: utf-8

from os import getenv
import pymssql

# server = getenv("PYMSSQL_TEST_SERVER")
# user = getenv("PYMSSQL_TEST_USERNAME")
# password = getenv("PYMSSQL_TEST_PASSWORD")

server = "127.0.0.1"
user = "sa"
password = "!peak123"

conn = pymssql.connect(server, user, password, "tempdb")
cursor = conn.cursor()

cursor.execute("""
-- 学生表
IF OBJECT_ID('Student', 'U') IS NOT NULL
    DROP TABLE Student
create table Student(
    id int not null identity(1, 1) primary key,
	name varchar(20) not null,
	number varchar(20) not null,
	sex  varchar(10) not null,
	class varchar(30) not null,
    major varchar(20) not null,
	balance int not null
)
create index StudentIndex on Student(id)

-- 计算机表
IF OBJECT_ID('Computer', 'U') IS NOT NULL
    DROP TABLE Computer
create table Computer(
    id int not null identity(1, 1) primary key,
    number varchar(10) not null,
    ip varchar(15) not null,
    state varchar(20) not null,
	computer_room_id int not null
)
create index ComputerIndex on Computer(id)

-- 管理员表
IF OBJECT_ID('Manager', 'U') IS NOT NULL
    DROP TABLE Manager
create table Manager(
    id int not null identity(1, 1) primary key,
	name varchar(20) not null,
    sex varchar(10) not null,
    age int not null,
    id_number varchar(20) not null,
	phone char(11) not null
)
create index ManagerIndex on Manager(id)

-- 机房表
IF OBJECT_ID('ComputerRoom', 'U') IS NOT NULL
    DROP TABLE ComputerRoom
create table ComputerRoom(
    id int not null identity(1, 1) primary key,
    name varchar(20) not null,
    manager_id int not null
)
create index ComputerRoomIndex on ComputerRoom(id)

-- 维修表
IF OBJECT_ID('Repair', 'U') IS NOT NULL
    DROP TABLE Repair
create table Repair(
    id int not null identity(1, 1) primary key,
    date varchar(20) not null,
    worker_name varchar(30),
	computer_id int not null,
    manager_id int not null
)
create index RepairIndex on Repair(id)

-- 上机表
IF OBJECT_ID('Operate', 'U') IS NOT NULL
    DROP TABLE Operate
create table Operate(
    id int not null identity(1, 1) primary key,
    date_start varchar(20) not null,
    date_end varchar(20) not null,
	student_id int not null,
    computer_id int not null,
    foreign key (student_id) references Student(id) on delete cascade
)
create index OperateIndex on Operate(id)

-- 充值表
IF OBJECT_ID('Recharge', 'U') IS NOT NULL
    DROP TABLE Recharge
create table Recharge(
    id int not null identity(1, 1) primary key,
    money int not null,
    date varchar(20) not null,
	student_id int not null,
    manager_id int not null,
    foreign key (student_id) references Student(id) on delete cascade
)
create index RechargeIndex on Recharge(id)

-- 消息表
IF OBJECT_ID('Message', 'U') IS NOT NULL
    DROP TABLE Message
create table Message(
    id int not null identity(1, 1) primary key,
    text varchar(100) not null,
    date varchar(20) not null,
	operate_id int not null
)
create index MessageIndex on Message(id)

-- 学生数据
insert into Student(name, number, sex, class, major, balance) VALUES ('瀛︾敓A', '0001', '鐢�', '杞�?1(1)', '杞�?', 0)
insert into Student(name, number, sex, class, major, balance) VALUES ('瀛︾敓B', '0002', '濂�', '杞�?1(1)', '杞�?', 0)
insert into Student(name, number, sex, class, major, balance) VALUES ('瀛︾敓C', '0003', '濂�', '杞�?1(1)', '杞�?', 0)

-- 管理员数据
insert into Manager(name, sex, age, id_number, phone) VALUES ('绠＄悊鍛�?', '鐢�', 28, '411323199001016950', '17011111234')
insert into Manager(name, sex, age, id_number, phone) VALUES ('绠＄悊鍛�?', '濂�', 26, '411323199201016950', '17011115678')

-- 机房数据
insert into ComputerRoom(name, manager_id) VALUES ('鏈烘�?1', 1)
insert into ComputerRoom(name, manager_id) VALUES ('鏈烘�?1', 1)
insert into ComputerRoom(name, manager_id) VALUES ('鏈烘�?1', 1)
insert into ComputerRoom(name, manager_id) VALUES ('鏈烘�?2', 2)
insert into ComputerRoom(name, manager_id) VALUES ('鏈烘�?2', 2)
insert into ComputerRoom(name, manager_id) VALUES ('鏈烘�?2', 2)

-- 计算机数据
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0001', '192.168.1.1', 'normal', 1)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0002', '192.168.1.2', 'normal', 1)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0003', '192.168.1.3', 'normal', 1)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0004', '192.168.1.4', 'normal', 2)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0005', '192.168.1.5', 'normal', 2)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0006', '192.168.1.6', 'bad', 2)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0007', '192.168.1.7', 'bad', 2)

-- 维修数据
insert into Repair(date, worker_name, computer_id, manager_id) VALUES ('201807011513', '缁翠慨浜哄憳A', 6, 2)
insert into Repair(date, worker_name, computer_id, manager_id) VALUES ('201807011513', '缁翠慨浜哄憳A', 7, 2)

-- 上机数据
insert into Operate(date_start, date_end, student_id, computer_id) VALUES ('201807011513', '201807011700', 1, 1)
insert into Operate(date_start, date_end, student_id, computer_id) VALUES ('201806280900', '201806281130', 2, 2)

-- 充值数据
insert into Recharge(money, date, student_id, manager_id) VALUES (100, '201806281130', 1, 1)
insert into Recharge(money, date, student_id, manager_id) VALUES (100, '201806281130', 2, 1)


""")
cursor.executemany(
    "insert into Student(name, number, sex, class, major, balance) VALUES (%s, %s, %s, %s, %s, %d)",
    [
        ('John Smith', '0001', '鐢�', '杞�?1(1)', '杞�?', 0),
        ('Jane Doe', '0001', '濂�', '杞�?1(2)', '杞�?', 0),
        ('Mike T.', '0003', '濂�', '杞�?1(3)', '杞�?', 0)
    ]
)
# you must call commit() to persist your data if you don't set autocommit to True
conn.commit()

cursor.execute('SELECT * FROM Student WHERE id=%d', 1)
row = cursor.fetchone()
while row:
    print("id=%d, name=%s, number=%s" % (row[0], row[1], row[2]))
    row = cursor.fetchone()

# cursor.execute('SELECT * FROM persons WHERE salesrep=%s', 'John Doe')
# for row in cursor:
#     print('row = %r' % (row,))

conn.close()
