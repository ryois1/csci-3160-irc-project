const express = require("express");
const app = express();
const port = 5173;

app.use(express.static("dist"));

app.get("*", (req, res) => {
  res.sendFile(__dirname + "/dist/index.html");
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});