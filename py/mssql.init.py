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

-- 初始化工作, 便于多次执行, 调试使用

IF OBJECT_ID('Operate', 'U') IS NOT NULL
    DROP TABLE Operate

IF OBJECT_ID('Recharge', 'U') IS NOT NULL
    DROP TABLE Recharge

IF OBJECT_ID('Student', 'U') IS NOT NULL
    DROP TABLE Student

IF OBJECT_ID('Computer', 'U') IS NOT NULL
    DROP TABLE Computer

IF OBJECT_ID('Manager', 'U') IS NOT NULL
    DROP TABLE Manager

IF OBJECT_ID('ComputerRoom', 'U') IS NOT NULL
    DROP TABLE ComputerRoom

IF OBJECT_ID('Repair', 'U') IS NOT NULL
    DROP TABLE Repair

IF OBJECT_ID('Message', 'U') IS NOT NULL
    DROP TABLE Message

-- 1, 首先创建基本的表结构

-- 学生表
create table Student(
    id int not null identity(1, 1) primary key,
	name varchar(20) not null,
	number varchar(20) not null,
	sex  varchar(10) not null,
	class varchar(30) not null,
    major varchar(20) not null,
	balance int not null
)

-- 计算机表
create table Computer(
    id int not null identity(1, 1) primary key,
    number varchar(10) not null,
    ip varchar(15) not null,
    state varchar(20) not null,
	computer_room_id int not null
)

-- 管理员表
create table Manager(
    id int not null identity(1, 1) primary key,
	name varchar(20) not null,
    sex varchar(10) not null,
    age int not null,
    id_number varchar(20) not null,
	phone char(11) not null
)

-- 机房表
create table ComputerRoom(
    id int not null identity(1, 1) primary key,
    name varchar(20) not null,
    manager_id int not null
)

-- 维修表
create table Repair(
    id int not null identity(1, 1) primary key,
    date varchar(20) not null,
    worker_name varchar(30),
	computer_id int not null,
    manager_id int not null
)

-- 上机表
create table Operate(
    id int not null identity(1, 1) primary key,
    date_start varchar(20) not null,
    date_end varchar(20) not null,
	student_id int not null,
    computer_id int not null,
    foreign key (student_id) references Student(id) on delete cascade
)

-- 充值表
create table Recharge(
    id int not null identity(1, 1) primary key,
    money int not null,
    date varchar(20) not null,
	student_id int not null,
    manager_id int not null,
    foreign key (student_id) references Student(id) on delete cascade
)

-- 消息表
create table Message(
    id int not null identity(1, 1) primary key,
    text varchar(100) not null,
    date varchar(20) not null,
	operate_id int not null
)


-- 2, 创建索引

create index StudentIndex on Student(id)
create index ComputerIndex on Computer(id)
create index ManagerIndex on Manager(id)
create index ComputerRoomIndex on ComputerRoom(id)
create index RepairIndex on Repair(id)
create index OperateIndex on Operate(id)
create index RechargeIndex on Recharge(id)
create index MessageIndex on Message(id)


-- 3, 创建触发器

-- 新增一条充值记录时, 更新该学生的余额
CREATE TRIGGER trigger_Recharge_insert
    ON Recharge
    AFTER INSERT
AS
BEGIN
    declare @student_id int;
    declare @count int;
    select @student_id = student_id from inserted;
    select @count = count(*) from Student where student_id=@student_id;
    if(@count = 1)
        BEGIN
            update Student SET column_name1 = VALUE WHRER column_name2 = VALUE
        END

    INSERT iNTO B表(B表字段1,B表字段2,B表字段3)
    SELECT A表字段1,A表字段2,A表字段3
    FROM INSERTED
END
GO



-- 学生数据
insert into Student(name, number, sex, class, major, balance) VALUES ('学生A', '0001', '男', '软件1(1)', '软件', 0)
insert into Student(name, number, sex, class, major, balance) VALUES ('学生B', '0002', '女', '软件1(1)', '软件', 0)
insert into Student(name, number, sex, class, major, balance) VALUES ('学生C', '0003', '女', '软件1(1)', '软件', 0)

-- 管理员数据
insert into Manager(name, sex, age, id_number, phone) VALUES ('管理员A', '男', 28, '411323199001016950', '17011111234')
insert into Manager(name, sex, age, id_number, phone) VALUES ('管理员B', '女', 26, '411323199201016950', '17011115678')

-- 机房数据
insert into ComputerRoom(name, manager_id) VALUES ('机房1', 1)
insert into ComputerRoom(name, manager_id) VALUES ('机房2', 2)

-- 计算机数据
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0001', '192.168.1.1', 'normal', 1)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0002', '192.168.1.2', 'normal', 1)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0003', '192.168.1.3', 'normal', 1)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0004', '192.168.1.4', 'normal', 2)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0005', '192.168.1.5', 'normal', 2)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0006', '192.168.1.6', 'bad', 2)
insert into Computer(number, ip, state, computer_room_id) VALUES ('C0007', '192.168.1.7', 'bad', 2)

-- 维修数据
insert into Repair(date, worker_name, computer_id, manager_id) VALUES ('201807011413', '维修员A', 6, 2)
insert into Repair(date, worker_name, computer_id, manager_id) VALUES ('201807011513', '维修员A', 7, 2)

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
        ('John Smith', '0001', '男', '软件1(1)', '软件', 0),
        ('Jane Doe', '0001', '女', '软件1(2)', '软件', 0),
        ('Mike T.', '0003', '女', '软件1(3)', '软件', 0)
    ]
)
# you must call commit() to persist your data if you don't set autocommit to True
conn.commit()

cursor.execute('SELECT * FROM Student WHERE id=%d', 1)
row = cursor.fetchone()
while row:
    print("id=%d, name=%s, number=%s" % (row[0], row[1], row[2]))
    row = cursor.fetchone()

# 卡上余额
cursor.execute('select balance from Student where name = %s', '学生A')
for row in cursor:
    print('name: %s, balance: %d' % ("学生A", row[0]))

conn.close()
