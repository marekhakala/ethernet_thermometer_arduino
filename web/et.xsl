<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
      <head>
        <title>Ethernet thermometer</title>
        <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" rel="stylesheet" integrity="sha256-7s5uDGW3AHqw6xtJmNNtr+OBRJUlgkNJEo78P4b0yRw= sha512-nNo+yCHEyn0smMxSswnf/OnX6/KwJuZTlNZBjauKhTK0c+zT+q5JOCx0UFhXQ6rJR9jg6Es8gPuD2uZcYDLqSw==" crossorigin="anonymous">
      </head>
      <body>
        <div class="container">
          <div class="row text-center">
            <h1>Ethernet thermometer</h1>
          </div>
          <div class="row text-center">
            <hr />
            <a href="/" class="btn btn-xs btn-default">Update</a></p>
            <hr />
            <table class="table table-striped table-bordered">
              <tr>
                <th>Celsius</th>
                <th>Fahrenheit</th>
                <th>Humidity</th>
              </tr>
              <tr>
                <td><xsl:value-of select="response/values/value[1]"/> &#176;C</td>
                <td><xsl:value-of select="response/values/value[2]"/> &#176;F</td>
                <td><xsl:value-of select="response/values/value[3]"/> %</td>
              </tr>
            </table>
          </div>
        </div>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
