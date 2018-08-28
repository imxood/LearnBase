# coding: utf-8

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

# 设置随机种子
np.random.seed(42)


# 设置全局横纵轴字体大小
mpl.rcParams['xtick.labelsize'] = 24
mpl.rcParams['ytick.labelsize'] = 24

numbers = np.random.rand(30)
print(numbers)

# x轴的采样点
x = np.linspace(0, 5, 100)

y = 2 * np.sin(x) + 0.3 * x ** 2
y_data = y + np.random.normal(scale=0.3, size=100)

# 指定图表名称
plt.figure("data")

# '.'标明画散点图，每个散点的形状是个圆
plt.plot(x, y_data)

# 画模型的图，plot函数默认画连线图
plt.figure("model")
plt.plot(x, y)


# 两个一起画
plt.figure("data & model")
plt.plot(x, y, 'k', lw=2)

plt.scatter(x, y_data)

plt.show()
