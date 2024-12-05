const statusDotElement = document.querySelector(".status-dot");
const statusTextElement = document.querySelector(".status-text");
const msgElement = document.querySelector(".msg");

async function getStatus() {
  try {
    const res = await fetch("/api/power/status")
    if (!res.ok) {
      statusDotElement.className = "status-dot unknown"
      statusTextElement.innerHTML = "Unknown"
      throw new Error("Error fetching data " + res.status)
    }
    const status = await res.text();
    if (status.toLowerCase() == "up") {
      statusDotElement.className = "status-dot up"
    } else {
      statusDotElement.className = "status-dot down"
    }
    statusTextElement.innerHTML = status
  } catch (error) {
    console.error(error);
  }
}
getStatus()
setInterval(getStatus, 1000);

async function handleSubmit(e, url) {
  e.preventDefault();
  try {
    const res = await fetch(url, {
      method: "post"
    })
    const text = await res.text();
    console.log(text);
    msgElement.innerHTML = text
  } catch (error) {
    msgElement.innerHTML = text
    console.error(error);
  }
  setTimeout(() => {
    msgElement.innerHTML = ""
  }, 3000)
  return false;
} 
