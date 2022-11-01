import "./style.css";
import Module from "./src/test";

const module = await new Module();

const factorize = (n) => {
  const cres = module.factorize(n);

  const ckeys = cres.keys();
  const keys = [...new Array(ckeys.size())].map((_, idx) => ckeys.get(idx));
  ckeys.delete();

  const res = keys.reduce((red, key) => ({ ...red, [key]: cres.get(key) }), {});
  cres.delete();

  return res;
};

const factorize_str = (n) => {
  const res = factorize(n);
  return Object.entries(res)
    .map((e) => (e[1] == 1 ? `${e[0]}` : `(${e[0]} ^ ${e[1]})`))
    .join(" * ");
};

const run = () => {
  const inp = document.getElementById("number_input");
  const n = parseInt(inp.value);
  const res = factorize_str(n);
  const out = document.getElementById("factorization_reuslt");
  out.innerText = `${n} = ${res}`;
};

run();

const button = document.getElementById("factorize_button");
button.addEventListener("click", run);
