# platform
 基础平台库，提供稳定接口，避免重复实现

## bin

可执行文件夹，包含.a .dll .exe等文件

## examples

示例文件夹

## src

源文件目录

### PfAdapter

适配器相关：网络、串口、模拟量输入、模拟量输出、数字量输入、数字量输出等适配器

向上层应用提供统一接口

### PfBus

总线相关：包括网络、串口、消息中间件等

### PfCommon

基础模块：包括动态创建类、TinyXml、日志存储等

### PfNi

NI相关板卡操作：包括模拟量、数字量、继电器等

### PfFlow

流程自动化运行：可根据XML描述自动化执行

### template

流程部创建模板

适配器创建模板