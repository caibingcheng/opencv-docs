### saliency 模块demo

#### 运行：

```shell
#SPECTRAL_RESIDUAL method
./demo SPECTRAL_RESIDUAL ../static/vtest.avi 1

#FINE_GRAINED method
./demo FINE_GRAINED ../static/vtest.avi 1

#BING method
./demo BING ../static/vtest.avi 1 ../static/bing_models

#BinWangApr2014 method
./demo BinWangApr2014 ../static/vtest.avi 1
```

###### video可选项：

- ../static/vtest.avi
- ../static/video0.avi
- ../static/video1.avi

#### 其他（可选）：

【可选】：https://gitee.com/jerry323/cbuild.git 或 https://github.com/caibingcheng/cbuild.git 提供C++项目管理工具，可用于创建、删除、编译或运行C++项目。