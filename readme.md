# Smart Parking

Ứng dụng C++ mô phỏng quản lý bãi đỗ xe trên terminal: tiếp nhận xe vào, xử lý xe ra, phân bổ chỗ đỗ và quản lý hàng chờ khi bãi đầy. Cảm biến và barrier được mô phỏng bằng phần mềm.

## Chức năng

- Bãi đỗ mặc định có 10 chỗ, cấp phát chỗ trống đầu tiên theo thứ tự P1 đến P10.
- Kiểm tra định dạng mã xe; từ chối xe đã có trong bãi hoặc yêu cầu ra của xe không tồn tại.
- Đưa xe vào hàng chờ FIFO khi hết chỗ; thử tiếp nhận một xe đầu hàng sau mỗi lượt xe ra thành công.
- Hiển thị số chỗ trống, chỗ đã sử dụng, mã xe, thời điểm vào và thời gian đỗ.
- Chạy bằng menu, sự kiện ngẫu nhiên hoặc file script.
- Ghi sự kiện ra terminal và nối thêm vào `logs/system.log`.

## Biên dịch

Cần trình biên dịch hỗ trợ C++. Chạy các lệnh từ thư mục gốc dự án.

### Windows — PowerShell và MinGW-w64

```powershell
$sources = Get-ChildItem src -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp $sources -o smart_parking.exe
New-Item -ItemType Directory -Force logs | Out-Null
.\smart_parking.exe
```
## Sử dụng

### Menu

```powershell
.\smart_parking.exe
```

| Lựa chọn | Thao tác |
| --- | --- |
| `1` | Nhập mã xe vào bãi |
| `2` | Nhập mã xe ra khỏi bãi |
| `3` | Xem trạng thái bãi |
| `0` | Thoát |

Mã xe ví dụ: `28H-0001`, `30A-12345`, `30F-256.58`. Mã xe phải viết hoa đúng định dạng, không có khoảng trắng ở đầu hoặc cuối. Chương trình so sánh nguyên chuỗi: `30A-12345` và `30A-123.45` là hai mã khác nhau.

### Chế độ tự động

```powershell
.\smart_parking.exe --auto 10 --interval 500
```

Sinh 10 sự kiện vào/ra. Khi bãi có xe, mỗi sự kiện có xác suất 40% chọn một xe đang đỗ để ra; nếu không chọn xe ra thì tạo mã xe mới để vào. Khi bãi trống, sự kiện luôn là xe vào. Kết quả thay đổi giữa các lần chạy.

`--interval` là thời gian nghỉ giữa các sự kiện tự động, tính bằng mili giây; thời gian xử lý barrier và xe đi qua được cộng thêm. Giá trị `0` không bỏ qua các độ trễ này.

### Chế độ script

```powershell
.\smart_parking.exe --script scripts/demo.txt --interval 500
.\smart_parking.exe --help
```

| Lệnh | Ý nghĩa |
| --- | --- |
| `ENTRY <ID>` | Yêu cầu xe vào |
| `EXIT <ID>` | Yêu cầu xe ra |
| `STATUS` | Hiển thị trạng thái |
| `WAIT <MS>` | Nghỉ số mili giây chỉ định |
| `AUTO <N>` | Sinh N sự kiện tự động |
| `QUIT` | Dừng đọc script |

Lệnh phân biệt chữ hoa/chữ thường. Dòng trống và phần sau `#` được bỏ qua; hỗ trợ BOM UTF-8 đầu file. Các giá trị số phải là số nguyên không âm trong phạm vi `int`.

```text
ENTRY 30A-123.45
WAIT 1000
STATUS
EXIT 30A-123.45
STATUS
QUIT
```

Không kết hợp `--auto` và `--script`. Lỗi tham số, lỗi mở file hoặc lỗi cú pháp script trả mã thoát `1`; script dừng tại dòng lỗi. Yêu cầu đúng cú pháp nhưng bị từ chối do nghiệp vụ, như xe không tồn tại, không làm script dừng. Các thao tác trước dòng lỗi vẫn đã được thực hiện.

## Cấu hình

Các hằng số nằm trong [include/Config.h](include/Config.h); cần biên dịch lại sau khi thay đổi.

| Hằng số | Mặc định | Ý nghĩa |
| --- | --- | --- |
| `DEFAULT_TOTAL_SLOTS` | 10 | Số chỗ đỗ |
| `AUTO_INTERVAL_MS` | 500 | Thời gian nghỉ giữa sự kiện tự động |
| `BARRIER_MOVEMENT_MS` | 1000 | Độ trễ mô phỏng chuyển động barrier |
| `VEHICLE_PASSAGE_MS` | 1000 | Độ trễ mô phỏng xe đi qua |
| `UTC_OFFSET_MINUTES` | 420 | Hiển thị thời gian UTC+7 |

## Cấu trúc dự án

```text
main.cpp                 Điểm vào chương trình
include/                 Khai báo lớp và cấu hình
src/app/                 Tham số dòng lệnh và menu
src/controller/          Điều phối xe vào/ra, chỗ đỗ và hàng chờ
src/parking/             Dữ liệu và trạng thái chỗ đỗ
src/sensor/              Kiểm tra mã xe, sinh sự kiện tự động
src/barrier/             Mô phỏng barrier
src/display/             Hiển thị terminal
src/loging/              Ghi log
src/script/              Đọc và thực thi script
src/utils/               Định dạng thời gian
scripts/demo.txt         Kịch bản chạy mẫu
logs/system.log          Nhật ký sự kiện
doc/                     Tài liệu yêu cầu và sơ đồ
```

## Giới hạn hiện tại

- `Barrier::open()` đang gán `CLOSED`; `close()` đang gán `OPEN` khi đi qua nhánh xử lý. Barrier khởi tạo ở `CLOSED` nên chu trình thông thường vẫn báo `CLOSED`, dù có log `OPENED`. Hai trạng thái trung gian chưa được gán trong mã nguồn.
- Hàng chờ chưa kiểm tra mã xe trùng, chưa có giới hạn kích thước và chưa hỗ trợ hủy lượt chờ.
- Trạng thái bãi chỉ tồn tại trong bộ nhớ, không phục hồi từ log khi khởi động lại. Chưa có tính phí, cảnh báo quá giờ, cơ sở dữ liệu hay kết nối thiết bị thật.
- Logger không tự tạo thư mục `logs` và không báo lỗi ra file nếu không mở được file. Nội dung log hiện chưa có timestamp.
- Chưa có bộ kiểm thử tự động tại `tests/test_main.cpp`; target `make test` và nhánh kiểm thử của CMake cần file này để hoạt động.

## Tài liệu

- [Yêu cầu hệ thống](doc/Requriement.md)
- [Sơ đồ khối](doc/blockdiagram.jpg)
- [Sơ đồ trạng thái xe vào](doc/state_xe_vao.jpg)
- [Sơ đồ trạng thái xe ra](doc/state_xe_ra.jpg)

