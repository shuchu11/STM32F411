import numpy as np

# 讀取 16-bit PCM 原始資料（改成你實際的檔名）
data = np.fromfile("gummyB.pcm", dtype=np.int16)

# 如果你需要轉成 uint16（可選）
# data = data.astype(np.uint16)

# 轉成 C 語言陣列格式的字串
array_str = ", ".join(map(str, data))

# 輸出為 C 程式碼
c_code = f"""#include <stdint.h>

const int16_t audio_data[] = {{
    {array_str}
}};
const uint32_t audio_length = {len(data)};
"""

with open("gummyB_data.c", "w") as f:
    f.write(c_code)
