# Đặc tả yêu cầu Smart Parking

## 1. Mục tiêu và phạm vi

Smart Parking mô phỏng hoạt động của một bãi đỗ xe bằng C++17 trên terminal. Hệ thống quản lý chỗ đỗ, xác thực mã xe, điều phối xe vào/ra qua hai barrier và tiếp nhận xe chờ khi có chỗ trống.

Đầu vào đến từ người vận hành qua menu, file script hoặc bộ sinh sự kiện tự động. Cảm biến chỉ kiểm tra chuỗi mã xe, không nhận tín hiệu phần cứng hoặc xử lý hình ảnh. Dữ liệu bãi đỗ được lưu trong bộ nhớ của tiến trình.

## 2. Thuật ngữ và dữ liệu

| Thành phần         | Mô tả                                                                |
| ------------------ | -------------------------------------------------------------------- |
| Vehicle ID         | Chuỗi mã xe được dùng làm khóa tra cứu; giữ nguyên định dạng đầu vào |
| Parking slot       | Chỗ đỗ có ID từ 1 đến N, trạng thái `FREE` hoặc `OCCUPIED`           |
| Entry/Exit sensor  | Bộ kiểm tra mã xe tại luồng vào/ra                                   |
| Entry/Exit barrier | Barrier mô phỏng cho từng chiều di chuyển                            |
| Waiting queue      | Hàng chờ FIFO của các yêu cầu vào khi bãi đầy                        |

Mỗi chỗ đỗ lưu ID, trạng thái, mã xe và thời điểm vào kiểu `time_t`. Khi giải phóng chỗ, mã xe được xóa và thời điểm vào trở về `0`. Ánh xạ `vehicleToSlot` liên kết mã xe đang đỗ với ID chỗ đỗ.

## 3. Yêu cầu chức năng

| Mã    | Yêu cầu                                                                                                   | Thành phần thực hiện                    |
| ----- | --------------------------------------------------------------------------------------------------------- | --------------------------------------- |
| FR-01 | Khởi tạo N chỗ trống đánh số liên tiếp từ 1; ứng dụng mặc định N = 10                                     | `ParkingController`, `Config`           |
| FR-02 | Kiểm tra mã xe trước khi xử lý cả chiều vào và chiều ra                                                   | `EntrySensor`, `ExitSensor`             |
| FR-03 | Từ chối xe vào nếu mã xe đang tồn tại trong bãi                                                           | `ParkingController`                     |
| FR-04 | Cấp chỗ trống đầu tiên, lưu thời điểm vào và ánh xạ mã xe khi tiếp nhận thành công                        | `ParkingController`, `ParkingSlot`      |
| FR-05 | Khi bãi đầy, đóng barrier vào theo yêu cầu điều khiển, thêm mã xe vào hàng chờ và trả `REJECTED_FULL`     | `ParkingController`                     |
| FR-06 | Từ chối yêu cầu ra nếu mã xe không có trong bãi                                                           | `ParkingController`                     |
| FR-07 | Khi xe ra, giải phóng chỗ, xóa ánh xạ và ghi thời gian đỗ theo giây                                       | `ParkingController`, `ParkingSlot`      |
| FR-08 | Sau một lượt ra thành công, nếu có chỗ trống và hàng chờ không rỗng, lấy một xe đầu hàng để thử tiếp nhận | `ParkingController`                     |
| FR-09 | Điều khiển barrier theo chu trình đóng → đang mở → mở → đang đóng → đóng                                  | `Barrier`; chưa đáp ứng đúng trạng thái |
| FR-10 | Hiển thị tổng chỗ, số chỗ đã dùng/còn trống, từng chỗ đỗ, xe đang đỗ, thời gian và trạng thái barrier     | `Display`, `TimeUtils`                  |
| FR-11 | Hiển thị độ dài hàng chờ khi có xe đang chờ                                                               | `Display`                               |
| FR-12 | Cung cấp menu vào/ra/xem trạng thái/thoát                                                                 | `ParkingApplication`                    |
| FR-13 | Sinh N sự kiện tự động, có khoảng nghỉ cấu hình được                                                      | `AutoSensor`                            |
| FR-14 | Thực thi script với `ENTRY`, `EXIT`, `STATUS`, `WAIT`, `AUTO`, `QUIT`                                     | `ScriptRunner`                          |
| FR-15 | Ghi sự kiện `ENTRY`, `EXIT`, `BARRIER`, `ERROR`, `INFO` ra terminal và thử ghi nối vào file log           | `Logger`                                |
| FR-16 | Kiểm tra tham số khởi động; báo lỗi và trả mã thoát khác 0 khi tham số hoặc script sai                    | `ParkingApplication`, `ScriptRunner`    |

### 3.1. Quy tắc mã xe

- Dài 8–10 ký tự; hai ký tự đầu là chữ số, ký tự thứ ba là chữ hoa, ký tự thứ tư là `-`.
- Phần sau dấu `-` có dạng bốn chữ số, năm chữ số liền nhau hoặc năm chữ số dạng `123.45`.
- Các mã đầu được chấp nhận trong chương trình: 11–12, 14–38, 43, 47–79, 81–86, 88–90, 92–95 và 97–99. Đây là danh sách kiểm tra trong mã nguồn, không phải chức năng tra cứu đăng ký xe.
- Không tự cắt khoảng trắng, đổi chữ thường thành chữ hoa hoặc chuẩn hóa dấu chấm.
- Các chuỗi `30A-12345` và `30A-123.45` được xem là hai mã khác nhau.

### 3.2. Luồng xe vào

1. Sensor kiểm tra mã xe; sai định dạng trả `REJECTED_INVALID_INPUT`.
2. Nếu mã xe đã có trong `vehicleToSlot`, trả `REJECTED_DUPLICATE_VEHICLE`.
3. Tìm chỗ trống đầu tiên. Nếu hết chỗ, gọi đóng barrier vào, thêm xe vào hàng chờ và trả `REJECTED_FULL`.
4. Nếu còn chỗ, gọi mở barrier vào và chờ thời gian mô phỏng xe đi qua.
5. Đánh dấu chỗ đã sử dụng, lưu mã xe/thời điểm vào, cập nhật ánh xạ và ghi log.
6. Gọi đóng barrier, kiểm tra giới hạn số chỗ và trả `SUCCESS`.

### 3.3. Luồng xe ra

1. Sensor kiểm tra mã xe; sai định dạng trả `REJECTED_INVALID_INPUT`.
2. Tra cứu xe; nếu không tồn tại, trả `REJECTED_VEHICLE_NOT_FOUND`.
3. Gọi mở barrier ra và chờ thời gian mô phỏng xe đi qua.
4. Tính thời gian đỗ, giải phóng chỗ, xóa ánh xạ và ghi log.
5. Gọi đóng barrier ra, kiểm tra giới hạn số chỗ.
6. Thử tiếp nhận một xe đầu hàng chờ nếu có chỗ; trả `SUCCESS` cho lượt xe ra.

### 3.4. Chạy tự động và script

Khi có xe trong bãi, bộ sinh tự động chọn sự kiện ra với xác suất 40%, sau đó chọn ngẫu nhiên một xe đang đỗ. Các trường hợp còn lại tạo sự kiện vào; mã sinh bắt đầu từ `30A-000.01`. Thời gian nghỉ chỉ đặt giữa các sự kiện, ngoài thời gian xử lý sự kiện.

Script thực thi tuần tự, bỏ qua dòng trống và phần chú thích sau `#`. Lỗi cú pháp, sai mã xe hoặc sai tham số dừng script tại dòng lỗi. Việc từ chối nghiệp vụ đối với một lệnh hợp lệ không dừng script. `QUIT` hoặc hết file kết thúc đọc lệnh; ứng dụng hiển thị trạng thái cuối.

## 4. Ràng buộc và yêu cầu phi chức năng

| Mã     | Nội dung                                                                                                      |
| ------ | ------------------------------------------------------------------------------------------------------------- |
| NFR-01 | Biên dịch theo C++17; các module sử dụng thư viện chuẩn C++                                                   |
| NFR-02 | Xử lý tuần tự; độ trễ mô phỏng chặn việc xử lý sự kiện tiếp theo                                              |
| NFR-03 | Thời điểm hiển thị theo UTC+7, dạng `YYYY-MM-DD HH:MM:SS`; thời gian đỗ tính bằng giây từ đồng hồ hệ thống    |
| NFR-04 | Tổng số chỗ bằng số chỗ đã dùng cộng số chỗ còn trống                                                         |
| NFR-05 | Trong luồng controller, mỗi mã xe đang đỗ ánh xạ đến một chỗ đang sử dụng                                     |
| NFR-06 | Log được nối thêm qua các lần chạy nếu mở file thành công; thư mục `logs` phải tồn tại trong thư mục làm việc |

`validateInvariants()` hiện chỉ ghi lỗi khi số chỗ đã dùng vượt tổng số chỗ hoặc số chỗ trống âm; hàm này chưa kiểm tra đầy đủ tính nhất quán giữa ánh xạ và từng chỗ đỗ.

## 5. Cấu hình mặc định

| Thông số                      | Giá trị |
| ----------------------------- | ------- |
| Số chỗ đỗ                     | 10      |
| Khoảng nghỉ tự động           | 500 ms  |
| Thời gian chuyển động barrier | 1000 ms |
| Thời gian xe đi qua           | 1000 ms |
| Độ lệch múi giờ               | UTC+7   |

Các giá trị nằm trong `include/Config.h`. `--interval` ghi đè khoảng nghỉ tự động trong lần chạy hiện tại. Hằng số giới hạn đỗ 8 giờ đã được khai báo nhưng chưa có xử lý áp dụng.

## 6. Tiêu chí kiểm tra thủ công

Các trường hợp dưới đây là tiêu chí kiểm tra, không phải báo cáo kết quả kiểm thử. Mỗi trường hợp bắt đầu bằng tiến trình mới, trừ khi nêu rõ chuỗi thao tác.

| Mã    | Thao tác                                             | Kết quả cần kiểm tra                                         |
| ----- | ---------------------------------------------------- | ------------------------------------------------------------ |
| AC-01 | Khởi động rồi xem trạng thái                         | 10 chỗ trống, 0 xe, hai barrier `CLOSED`                     |
| AC-02 | Cho `30A-123.45` vào                                 | Xe được cấp P1, số chỗ đã dùng là 1                          |
| AC-03 | Lặp lại mã xe của AC-02                              | Từ chối trùng; số chỗ đã dùng không đổi                      |
| AC-04 | Cho `abc` vào hoặc ra                                | Từ chối đầu vào, dữ liệu bãi không đổi                       |
| AC-05 | Cho xe hợp lệ chưa có trong bãi ra                   | Trả kết quả không tìm thấy xe                                |
| AC-06 | Cho đủ 10 xe khác nhau vào, sau đó thêm xe thứ 11    | 10 chỗ đã dùng, hàng chờ tăng 1                              |
| AC-07 | Từ AC-06, cho một xe đang đỗ ra                      | Xe đầu hàng được tiếp nhận vào chỗ vừa trống; hàng chờ còn 0 |
| AC-08 | Cho một xe vào, chờ rồi cho ra khi không có hàng chờ | Chỗ được giải phóng, log có thời gian đỗ                     |
| AC-09 | Script có lệnh không tồn tại                         | Báo số dòng lỗi, dừng và trả mã thoát 1                      |
| AC-10 | Chạy đồng thời `--auto 1 --script scripts/demo.txt`  | Báo không được kết hợp hai chế độ, trả mã thoát 1            |
| AC-11 | Quan sát chu trình barrier                           | Đối chiếu FR-09; hiện còn sai lệch như mục 7                 |

## 7. Sai lệch và giới hạn triển khai

- **Barrier:** `open()` gán `CLOSED`, `close()` gán `OPEN`; chưa gán `OPENING` hoặc `CLOSING`. Do khởi tạo ở `CLOSED`, lời gọi `close()` trong luồng thông thường trả về ngay. Log mở barrier không phản ánh đúng biến trạng thái.
- **Hàng chờ:** chỉ kiểm tra trùng với xe đang đỗ, không kiểm tra xe đã chờ. Một mã có thể xuất hiện nhiều lần trong hàng; mỗi lượt ra chỉ lấy một phần tử để thử tiếp nhận, không lặp đến khi nhận được xe hợp lệ.
- **Lưu trữ:** không lưu/khôi phục trạng thái bãi qua lần khởi động; log chưa có timestamp và lỗi mở log không được thông báo riêng.
- **Phạm vi:** chưa có thu phí, tài khoản, đặt chỗ, cảnh báo quá giờ, giao diện đồ họa hoặc tích hợp phần cứng.
