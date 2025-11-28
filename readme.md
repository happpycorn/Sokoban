# Sokoban

推箱子小遊戲 for 2025 Programing introduction

## 玩法

1. 將專案 clone 到本地
1. 在終端機執行

    ```bash
    ./sokoban
    ```

1. 你的角色是 '$'，可以透過輸入 W, A, S, D 來移動。遊戲目標是將所有的箱子（'@'）推到點上（'.'）。

## 自訂關卡

可以在運行時使用自己的關卡，只要將關卡內容打進 .txt 檔案，再於運行時作為參數即可。

關卡格式：

```txt
<row> <col>
<stage>
```

自訂關卡的使用方法：

```bash
./sokoban <stage_name>
```

如：

```bash
./sokoban Map/0_test_map.txt
```
