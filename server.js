const express = require("express");
const app = express();
const fetch = require("node-fetch");

const API_KEY = "Z7LK158AZEMLIJ2C";
var state = false;
app.get("/", (req, res) => {
  const { field1 } = req.query;
  console.log(field1);
  if(state){
      state=false;
      res.status(404);
  }
  fetch("https://api.thingspeak.com/update?apikey=${API_KEY}&field1=${field1}")
      .then(res => res.json()).then(res => console.log(res))
      .catch(e => console.log(e));
  res.json("Transmitted");
})

app.get("/stop", (req,res) => {
  state=true;
  console.log("Stop");
})

app.listen(3000, () => console.log("Listening on port 3000"));