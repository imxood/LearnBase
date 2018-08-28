drop trigger trigger_Recharge_insert
drop PROCEDURE Operate_start
drop PROCEDURE Operate_end
drop function GetTimeString
drop function GetTimeDiffMinute
-- 2, 创建触发器, 存储过程, 函数
    

GO

-- 新增一条充值记录时, 更新该学生的余额
CREATE TRIGGER trigger_Recharge_insert
    ON Recharge
    AFTER INSERT
AS
BEGIN
    DECLARE @student_id int;
    DECLARE @money int;
    DECLARE @count int;
    select @student_id = student_id, @money = money
    from inserted;
    -- 从插入的数据中获得学生id和money
    select @count = count(*)
    from Student
    where id = @student_id;
    if (@count = 1) -- 如果找到了这个人则追加ta的余额
    BEGIN
        update Student set balance=@money + (select balance from Student where id=@student_id)
            where id=@student_id
    END
END


GO
-- 计算格式化时间
CREATE FUNCTION GetTimeString()
RETURNS varchar(20)
BEGIN
    -- 先计算特定格式的当前时间
    DECLARE @time_string VARCHAR(20)
    
    select @time_string=CONVERT(varchar,GETDATE(),120)

    return @time_string
END

GO

-- 计算分钟差值
CREATE FUNCTION GetTimeDiffMinute(@date_start varchar(20), @date_end varchar(20))
RETURNS NUMERIC(20, 0)
BEGIN
    -- 先计算特定格式的当前时间
    DECLARE @time_diff_minute NUMERIC(20, 0)
    
    set @time_diff_minute = cast(datediff(n, @date_start, @date_end) as numeric(10,1))

    return @time_diff_minute
END


GO

-- 创建一个存储过程, 执行上机开始的操作: 记录date_start, 返回student_id
-- 输入参数: 学号 计算机id, 输出参数: 上机id 问候语
-- 返回值: 如果为1, 则学号不存在, 如果为2, 则余额不足1元,无法上机
-- 每小时1块钱计费
create procedure Operate_start(@student_number varchar(20), @computer_id int, @operate_id int output, @great_text varchar(100) output)
as
    DECLARE @UnitPrice INT
    set @UnitPrice=1 -- 单价, 1块钱1小时, 可以修改,若是2块钱,则有3块钱,只能玩3/2=1小时, 余数自动舍去

    DECLARE @count INT
    DECLARE @balance int            -- 余额
    DECLARE @date_start VARCHAR(20) -- 当前时间
    DECLARE @student_id VARCHAR(20) -- 学生id

    exec @date_start=GetTimeString 

    select @date_start

    -- 判断是否存在
    select @count = count(*) from Student where number=@student_number

    -- 根据学号没有找到这名学生
    IF (@count=0)
        set @great_text='学号错误, 请重新输入'
        return 1
    
    select @student_id=id, @balance=balance from Student where number=@student_number

    if(@balance < 2)
        set @great_text='你的余额为' + cast(@balance AS varchar(100)) + '元, 不足以上机, 请即时充值'
        return 2

    -- 保存数据
    insert into Operate (date_start, date_end, student_id, computer_id)
        values(@date_start, '', @student_id, @computer_id)

    -- 返回insert产生的自增id, 给上机结束时使用
    select @operate_id=@@identity

    set @great_text='你的余额为 ' + cast(@balance AS varchar(100)) + ' 元, 预计可使用 ' + cast((@balance / @UnitPrice) AS varchar(100)) + ' 小时, 请放心使用'

-- 测试
-- DECLARE @id int
-- exec Operate_start '0002', 1, @id output
-- select @id

GO

-- 创建一个存储过程, 执行上机结束的操作: 记录date_end, 同时给出消息Message
-- 输入参数: 上机id, 输出参数: 问候语
create procedure Operate_end(@operate_id int, @great_text varchar(100) output)
as
    DECLARE @date_start varchar(20)
    DECLARE @date_end varchar(20)

    DECLARE @during_minutes NUMERIC(20, 0)

    exec @date_end=GetTimeString

    select @date_start=date_start from Operate where id=@operate_id

    exec @during_minutes=GetTimeDiffMinute @date_start,@date_end

    -- 返回问候语
    set @great_text='你总共上机时间: ' + CAST(@during_minutes AS varchar(100)) + '分钟.'

GO

-- 测试
-- DECLARE @great_text varchar(100)
-- exec Operate_end 3, @great_text output
-- select @great_text


GO
-- 上机时, 查询该学生的余额
CREATE TRIGGER trigger_Operate_insert
    ON Operate
    AFTER INSERT
AS
BEGIN
    DECLARE @UnitPrice INT
    set @UnitPrice=1 -- 单价, 1块钱1小时, 可以修改,若是2块钱,则有3块钱,只能玩3/2=1小时, 余数自动舍去

    DECLARE @student_id int; -- 学生id
    DECLARE @date_start int; -- 开始时间
    DECLARE @date_end int;   -- 结束时间
    DECLARE @date_during NUMERIC(20, 0); -- 总共用时, 单位分钟
    DECLARE @date_hour int -- 总共用时, 单位小时, 向上取整, 如: 2.3小时取3

    -- 从插入的数据中获得学生id
    select @student_id = student_id, @date_start = date_start, @date_end = date_end from inserted;

    if(@date_end = '') -- 上机, 不做处理
        return

    -- 下机, 更新余额
    exec @date_during = GetTimeDiffMinute @date_start, @date_end
    set @date_hour = ceiling(@date_during / 60.0)
    
    update Student set balance=(select balance from Student where id=@student_id) - @date_hour * @UnitPrice
        where id=@student_id
END

-- 测试
-- select * from Student where balance > 0 --寻找余额大于0的学生

