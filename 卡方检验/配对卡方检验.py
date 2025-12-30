#不看主对角线，只看副对角线的差异
import pandas as pd
import statsmodels.stats.contingency_tables as tbl
#SquareTable类分析（常数值比较大，大于40）
table=tbl.SquareTable(pd.DataFrame([[10,20],[40,50]]))
print(table.summary())
#只查看配对卡方
print(table.symmetry())
#用mcnemar方法分析（常数值比较小，小于40）
table2=tbl.mcnemar(pd.DataFrame([[13,20],[10,12]]))
print(table2.pvalue)#p值大于0.05，接受原假设，无差异