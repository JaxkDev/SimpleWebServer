# RFC 1945 - HTTP/1.0   (also covers HTTP/0.9)


## HTTP/0.9

### Request
Does not include HTTP version or any other data in request except 'GET + [?path]'
Only accepted method is 'GET'.

### Response
Response only consists of the file requested, no name, metadata or headers to be included.


### Examples
Request Example:
```
GET /index.html
```

Response Example:
```html
<html>\r\n
Example HTML index page!\r\n
</html>
```