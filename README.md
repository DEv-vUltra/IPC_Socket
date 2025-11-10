<!doctype html>
<html lang="vi">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>Introduction to Sockets - README</title>
  <style>
    body{font-family:Inter, system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial; line-height:1.6; padding:28px; max-width:980px; margin:0 auto; color:#111}
    header{border-bottom:1px solid #e6e6e6; margin-bottom:20px}
    h1{font-size:28px}
    h2{font-size:20px; margin-top:22px}
    p, li{font-size:15px}
    pre{background:#f7f7f7;padding:12px;border-radius:6px;overflow:auto}
    img{max-width:100%;height:auto;border:1px solid #eaeaea;padding:6px;border-radius:6px;margin:12px 0}
    .note{background:#fffbe6;border-left:4px solid #ffd24d;padding:10px;margin:12px 0}
    ul{margin-left:20px}
    code{background:#f1f1f1;padding:2px 6px;border-radius:4px}
    footer{margin-top:30px;border-top:1px solid #eee;padding-top:12px;color:#666;font-size:13px}
  </style>
</head>
<body>
  <header>
    <h1>Introduction to Sockets</h1>
  </header>

  <article>

<p>Sockets là một trong những phương pháp để truyền dữ liệu giữa các applications diễn ra trên
cùng một máy chủ (localhost) hoặc trên các máy chủ khác thông qua một giao thức kết nối. Trong những trường hợp điển hình như server với client thì những applications giao tiếp với nhau bằng cách </p>

<ul>
  <li>Cả server và client được kernel cấp cho mỗi socket ( có thể hiểu giống như 1 điện thoại )</li>
  <li>Server sẽ cố định và ràng buộc socket của nó tới 1 địa chỉ xác định (có thể là 1 đường dẫn tuyệt đối ví dụ như /tmp/unix_socket) để client có thể xác định vị trí của nó</li>
</ul>

<p>Đa phần các hệ điều hành ngày nay đều hỗ trợ các miền (domain) như:</p>

<ul>
  <li>AF_UNIX: cho phép giao tiếp giữa các applications trong cùng 1 máy chủ</li>
  <li>IPv4 (AF_INET): cho phép giao tiếp giữa các máy chủ khác nhau thông qua giao thức mạng Internet version 4</li>
  <li>IPv6 (AF_INET6): cho phép giao tiếp giữa các máy chủ khác nhau thông qua giao thức mạng Internet version 6</li>
</ul>

<p>Về các loại socket thì có 2 loại phổ biến dùng trong cả UNIX và Internet protocol đó là stream và datagram</p>

<h2>Stream sockets(SOCK_STREAM)</h2>

<p>Stream sockets(SOCK_STREAM): cung cấp kênh dòng chảy thông tin đáng tin cậy và hai chiều</p>

<ul>
  <li>Đáng tin cậy( reliable ): Những thông tin được gửi đi bởi người gửi chắc chắn sẽ truyền tới người nhận (không tính trường hợp server hay client bị crashed)</li>
  <li>2 chiều (bidirectional): thông tin có thể truyền đi theo bất kì chiều nào giữa 2 sockets</li>
  <li>Kênh dòng chảy thông tin (Byte-stream): điều này nghĩa là sẽ không có khái niệm về tin nhắn(messages) hay sự ngăn cách tin nhắn (messages boundaries) khi nó được coi là 1 byte stream. Tiến trình sẽ đọc từ các khối data theo bất kì size nào. Ví dụ: nếu tôi muốn truyền 1 chuỗi string là "Hello world" thì nếu không có 1 bộ phân tách dữ liệu tức là xử lí chuỗi thì thông tin truyền đi có thể là "Hell oworld" hoặc bị dính liền lại thành "HelloWorld"</li>
</ul>

<img width="1009" height="820" alt="Screenshot from 2025-11-04 12-27-16" src="https://github.com/user-attachments/assets/0b18e2b4-fce4-4000-8986-78c625b57803" />



<p>Về quy trình hoạt động thì bạn có thể hiểu đơn giản như sau:</p>

<ul>
  <li>kernel sẽ cấp cho server và client mỗi người 1 cái điện thoại để trao đổi (tạo socket)</li>
  <li>Để client có thể biết được phải gọi ai thì phải cung cấp số điện thoại (bind)</li>
  <li>Sau đó để biết có ai đang gọi tới thì nghe giống như tiếng chuông điện thoại (listen)</li>
  <li>Việc có chấp nhận cuộc gọi cuộc gọi không thì sử dụng (accept)</li>
  <li>Sau đó thì có trao đổi thông tin với nhau (read/write)</li>
  <li>Kết thúc thì cả 2 cúp máy hoặc 1 trong 2 cúp máy (close)</li>
</ul>

<p>Trong quá trình gọi điện thì có thể có người bên thứ 3 gọi mình, lúc đấy phải đưa vào trạng thái chờ (pending). Tham số Backlog được sử dụng để quyết định giới hạn số lượng kết nối đang trong trạng thái chờ</p>

<img width="1093" height="568" alt="Screenshot from 2025-11-04 12-27-29" src="https://github.com/user-attachments/assets/0e2bed23-b2d7-4b9f-8609-0e0862659cf1" />


<h2>Datagram sockets(SOCK_DGRAM)</h2>

<p>Datagram sockets(SOCK_DGRAM): cho phép dữ liệu được trao đổi thông qua 1 dạng tin nhắn gọi là datagram. Với datagram thì các message boundaries sẽ được bảo toàn nhưng về sự truyền dữ liệu thì lại không ổn định. Điều này nghĩa là nếu đường truyền không ổn định thì nó sẽ truyền "Hello"(out of order) hoặc "Hello Hello" (hiện tượng lặp) hoặc là không truyền tới luôn.</p>

<img width="943" height="504" alt="Screenshot from 2025-11-04 12-27-53" src="https://github.com/user-attachments/assets/236ee3f9-da12-4a08-b06d-4efe35216bd3" />


<p>Về qui trình hoạt động thì datagram giống như việc gửi thư vào hòm thư</p>

<ul>
  <li>kernel sẽ cấp cho server và client 1 cái mailbox (tạo socket)</li>
  <li>Để client biết được địa chỉ mail thì server phải cung cấp địa chỉ (bind)</li>
  <li>Để gửi 1 mail hoặc đơn hàng thì phải gọi tới lệnh sendto(), trong mail phải có chứa địa  chỉ của server, giống như khi bạn viết thư</li>
  <li>Để nhận được mail thì phải gọi lệnh recvfrom()</li>
  <li>Khi gửi xong thì dùng hàm close()</li>
</ul>

<p>Việc gửi dữ liệu qua socket stream sẽ buộc phải bị đọc tuần tự từng cái nhưng đối với datagram socket thì không có gì đảm bảo rằng dữ liệu sẽ được đến tuần tự và đôi khi chưa chắc đã được gửi tới </p>

<h2>Về các system call của socket thì ta có:</h2>

<ul>
  <li>Socket(): tạo 1 socket mới</li>
  <li>Bind(): ràng buộc socket đó vào 1 địa chỉ </li>
  <li>listen(): cho phép 1 stream socket có thể "nghe" các kết nối đang tới từ các sockets khác</li>
  <li>connect(): thực hiện kết nối với 1 socket.</li>
</ul>

<p>Những chương trình được viết để mô phỏng server-client trong miền UNIX domain và IPv4 sử dụng socket
Đối với 2 chương trình server_no_delim.c và client_no_delim.c được sử dụng để quan sát khi mà chương trình không có sử dụng bộ phân tách dữ liệu kết quả sẽ giống như thế này:</p>

<img width="1395" height="329" alt="Screenshot from 2025-11-05 12-09-35" src="https://github.com/user-attachments/assets/ba6b0482-b2e2-40eb-9ab1-a55e8ce5df66" />


<p>Chuỗi "Hello World" thì chuyển thành "HelloWorld". Đây chỉ là một ví dụ đơn giản và dữ liệu không được gửi liên tục và nhanh. Giả sử nếu dữ liệu nhiều và liên tục thì nếu không có bộ phân tách dữ liệu thì output sẽ bị sai.</p>

<p>Thực tế thì 1 server sẽ phải giao tiếp với nhiều client khác nhau, điều này dẫn tới việc tạo ra quá nhiều luồng và server đôi khi sẽ không nhận được dữ liệu. Từ hình ảnh dưới đây bạn sẽ thấy được.

<img width="1801" height="338" alt="Screenshot from 2025-11-05 20-01-35" src="https://github.com/user-attachments/assets/bd256572-1ccc-4b9a-9328-7fc9e80a032d" />

<p>Hình ảnh trên là để quan sát hiện tượng khi mà nhiều client gửi tới 1 server nếu với chế độ blocking mặc định thì giả sử cilent 3 đang được accept và chờ dữ liệu từ client 3 thì nếu client 1 và 2 gửi trong quãng thời gian đó thì sẽ không trả về được giá trị vì đang còn bị kẹt ở client 3. Điều này khiến client bị treo và không thể được đóng lại vì không được phản hồi dẫn tới việc tiêu tốn tài nguyên.</p>

<p>Cách giải quyết vấn đề trên là sử dụng non-blocking socket, điều này cho phép nếu có dữ liệu thì cứ đọc/ghi như bình thường còn nếu không có dữ liệu thì sẽ trả về -1 hoặc errno = EAGAIN. Việc trả lại biến errno cho biết là dữ liệu chưa được sẵn sàng để bị đọc/ghi và sẽ quay lại thăm dò sau (polling) nhưng nếu thời gian thăm dò quá thưa thì sẽ gây ra độ trễ lớn và bị kéo dài ra, còn nếu quá dày thì sẽ lại tiêu tốn tài nguyên. Thế nên hãy sử dụng non-blocking socket kết hợp với select() hoặc poll().</p>

<img width="1801" height="338" alt="Screenshot from 2025-11-05 20-51-07" src="https://github.com/user-attachments/assets/e9c40822-c55a-4eea-9c7d-1814f8da04aa" />


<p>Trong việc sử dụng kiểu socket datagram thì thông thường đối với địa chỉ là AF_UNIX thì sẽ không xảy ra hiện tượng mất gói hay đảo lộn thứ tự gần như không xảy ra vì trong cùng 1 máy chủ (localhost) thì kernel kiểm soát việc đó rất tốt và tất cả dữ liệu đều đi qua bộ nhớ nội bộ của kernel chứ không thông qua mạng vật lí. Nhưng nếu liên kết qua các giao thức mạng như IPv4 hay IPv6 thì điều này sẽ xảy ra. </p>

<p>Đối với việc đảo lộn thứ tự thì hiện tượng này liên quan đến kiến thức về big/little endian. Ta được biết rằng trong các kiến trúc vi xử lí của AMD hay Intel đều tuân theo little endian thì khi truyền dữ liệu qua máy khác, thứ tự này có thể bị đảo ngược.</p>

<img width="1221" height="520" alt="Screenshot from 2025-11-05 21-29-16" src="https://github.com/user-attachments/assets/c0f12f1e-4862-41b1-9a17-021b330cd1e5" />


<p>Việc truyền dữ liệu thông qua 1 mạng phải tuân thủ 1 số nguyên tắc chung và network byte order là 1 trong số chúng và được quy định là big endian. Nhưng để có thể hiểu được những nguyên tắc chung này ta cần phải quay về những khái niệm cơ bản về Networking Protocols và Layers</p>

<p>Giao thức mạng (Networking Protocols) là 1 tập hợp các quy tắc định nghĩa cách mà thông tin được truyền đi thông qua mạng. Các giao thức mạng được tổ chức như 1 chuỗi các lớp (layers) với mỗi layer được tạo ra để có thể chứa những đặc trưng riêng để có thể tiếp cận tới những layer cao hơn.</p>

<img width="1053" height="867" alt="Screenshot from 2025-11-06 10-49-21" src="https://github.com/user-attachments/assets/20d84fab-e537-4694-8776-e9909006e2c7" />


<p>Ở bài viết ta sẽ chỉ nói đến Transport layer (tầng vận chuyển)</p>

<p>Có 2 giao thức phổ biến được dùng ở tầng vận chuyển là</p>

<ul>
  <li>User Datagram Protocol (UDP): là phương thức dùng cho datagram sockets</li>
  <li>Transmission Control Protocol (TCP): là phương thức dùng cho stream sockets</li>
</ul>

<p>Ngoài ra ta cũng sẽ đề cập 1 chút về Port Numbers: khái niệm này cho phép phân biệt các applications trên cùng 1 host giống như 1 căn nhà thì có nhiều phòng và mỗi phòng có 1 cửa thì cửa đó chính là port numbers.</p>

<h2>Về TCP</h2>

<p>Về TCP thì nó cung cấp:</p>

<ul>
  <li>Sự đáng tin cậy (reliable): có kiểm soát lỗi, thứ tự và truyền lại byte đã mất.</li>
  <li>Stream- oriented: coi dữ liệu là luồng byte liên tục, không phân chia</li>
</ul>

<img width="942" height="406" alt="Screenshot from 2025-11-08 11-28-07" src="https://github.com/user-attachments/assets/e3f6eeb5-eb38-4e00-b7a0-15220a41e41c" />


<p>Điều làm cho TCP phù hợp với stream sockets chính là việc nó có các cơ chế đảm bảo như Acknowledgements, retransmissions và timeouts cũng như là flow và congestion control để ngăn chặn sự tắc nghẽn nếu luồng dữ liệu được gửi đi quá nhanh. </p>

<p>Ta thấy rằng TCP đang tin cậy có thể kiểm soát lỗi và thứ tự cũng như truyền lại byte đã mất nhưng tại sao chúng ta lại vẫn sử dụng UDP với datagram socket. Tôi sẽ đưa ra 1 vài nhận xét dưới đây </p>

<ul>
  <li>1 UDP server có thể giao tiếp với nhiều client mà không cần phải tạo ra hay hủy 1 kết nối cho mỗi client. Còn đối với TCP thì với mỗi kết nối giữa client và server thì phải tạo 1 socket riêng -> điều này cũng dẫn tới việc gây tốn tài nguyên</li>
  <li>Và đối với ứng dụng yêu cầu và phản hồi đơn giản thì việc sử dụng UDP nhanh hơn rất nhiều so với TCP, đặc biệt là đối với các gói dữ liệu nhỏ và nhanh giống như video, game.</li>
</ul>

<p>Có 1 vài lưu ý khi test tốc độ truyền dữ liệu của 2 chương trình này:</p>

<ul>
  <li>Các câu lệnh printf() sẽ làm giảm tốc độ truyền dữ liệu vì khi printf() cho mỗi gói thì CPU sẽ mất thời gian để flush ra terminal</li>
  <li>Hãy chủ động tăng buffer_size của cả 2 chương trình udp_server và tcp_server ngang nhau để tránh việc bị nghẽn tại buffer </li>
  <li>Đối với chương trình udp_client_speed_compare.c thì việc truyền dữ liệu "hello" được gom lại vào 1 buffer và gửi đi.  Bản chất của UDP với datagram socket là gửi dữ liệu theo từng dạng gói (packages). Việc dồn vào 1 buffer cho thấy CPU ít phải system call và context switch hơn</li>
  <li>Ở chế độ thông thường tcp_server được gắn với cơ chế Nagle còn được biết đến là congestion and nagle algorithm. Có thể hiểu Nagle algorithm hoạt động bằng cách là gom nhiều gói nhỏ vào một gói lớn hơn và gửi đi để tránh hiện tượng overhead mạng. Việc phải gom nhiều gói nhỏ vào 1 gói lớn sẽ tốn 1 khoảng thời gian chờ nhất định. Thế nên tôi sẽ so sánh cả hai trường hợp: sử dụng Nagle và không sử dụng Nagle </li>
</ul>

<p>Có Nagle:</p>

<img width="1510" height="1064" alt="Screenshot from 2025-11-09 23-30-54" src="https://github.com/user-attachments/assets/44e7c624-01b2-49b8-a14c-a19822637494" />


<p>Không có Nagle:</p>

<img width="1582" height="1089" alt="Screenshot from 2025-11-10 12-15-37" src="https://github.com/user-attachments/assets/97134b11-c763-4c61-9b73-0cc0a6c2dfde" />

<p>Tốc độ nếu không có Nagle thì TCP sẽ nhanh hơn hẳn nhưng điều này sẽ gây ra lỗi khi truyền dữ liệu và không mang tính đảm bảo.  Hãy chỉnh sửa sử dụng html để có bố cục phù hợp với 1 file README và không được phép thay đổi nội dung</p>

  </article>

  <footer>README generated as HTML for viewing/editing. Images keep original paths as in source content.</footer>
</body>
</html>
