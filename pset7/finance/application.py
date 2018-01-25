from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    users = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])
    user = users[0]
    holdings = db.execute("SELECT symbol, SUM(quantity) FROM user_stocks WHERE user_id = :user_id GROUP BY symbol", user_id=session['user_id'])
    # print(holdings)
    # Do some processing before handing it in to jinja tempalte
    for holding in holdings:
        holding['total_quantity'] = holding['SUM(quantity)']
        holding['current_price'] = lookup(holding["symbol"])["price"]
        holding['total_value'] = holding['total_quantity']*holding['current_price']

    return render_template("index.html", user=user, holdings=holdings)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")
    if request.method == "POST":
        stock_to_buy = request.form.get("ticker")
        quantity_to_buy = request.form.get("quantity")
        current_price = lookup(stock_to_buy)["price"]
        user = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])[0]
        user_cash = user["cash"]
        if user_cash < int(quantity_to_buy)*current_price:
            return apology("You don't have enough money")
        db.execute("INSERT INTO 'user_stocks' (user_id, symbol, avg_price, quantity) VALUES (:user_id, :symbol, :avg_price, :quantity)", user_id=session["user_id"], symbol=stock_to_buy, avg_price=current_price, quantity=quantity_to_buy)
        # TODO: change the database and implement the purchasing process.
        new_cash_balance = user_cash - int(quantity_to_buy)*current_price
        db.execute("UPDATE users SET cash = :new_cash_balance WHERE id = :user_id", new_cash_balance=new_cash_balance, user_id=session["user_id"])
        return redirect(url_for("index"))
    return apology("Somthing is wrong")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "GET":
        transactions = db.execute("SELECT symbol, update_time, avg_price, quantity FROM user_stocks WHERE user_id = :user_id", user_id=session['user_id'])
        return render_template("history.html", transactions=transactions)
    return apology("Something is wrong here")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if request.method == "POST":
        ticker = request.form.get("ticker")
        information = lookup(ticker)
        if information is not None:
            return render_template("quoted.html", information = information)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "GET":
        return render_template("register.html")
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
        # query database for username
        else:
            row_insertion = db.execute("INSERT INTO 'users' (username, hash) VALUES (:username,:new_hash)", username=request.form.get("username"), new_hash=pwd_context.hash(request.form.get("password")))
            return redirect(url_for("login"))


    return apology("something went wrong with the development.")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "GET":
        users = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])
        user = users[0]
        holdings = db.execute("SELECT symbol, SUM(quantity) FROM user_stocks WHERE user_id = :user_id GROUP BY symbol", user_id=session['user_id'])
        # Do some processing before handing it in to jinja tempalte
        for holding in holdings:
            holding['total_quantity'] = holding['SUM(quantity)']
            holding['current_price'] = lookup(holding["symbol"])["price"]
            holding['total_value'] = holding['total_quantity']*holding['current_price']
        return render_template("sell.html", user=user, holdings=holdings)
    if request.method == "POST":
        users = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])
        user = users[0]
        holdings = db.execute("SELECT symbol, SUM(quantity) FROM user_stocks WHERE user_id = :user_id GROUP BY symbol", user_id=session['user_id'])
        stock_to_sell = request.form.get("stock_to_sell")
        #Not sure how should I get the quantatity availble for the sale.
        quantity_to_sell = next(item for item in holdings if item["symbol"] == stock_to_sell)['SUM(quantity)']
        current_price = lookup(stock_to_sell)["price"]
        # the sql table "users_stocks" are all records of transactions it is a history record.
        db.execute("INSERT INTO 'user_stocks' (user_id, symbol, avg_price, quantity) VALUES (:user_id, :symbol, :avg_price, :quantity)", user_id=session["user_id"], symbol=stock_to_sell, avg_price=current_price, quantity=-quantity_to_sell)
        new_cash_balance = quantity_to_sell*current_price + user['cash']
        db.execute("UPDATE users SET cash = :new_cash_balance WHERE id = :user_id", new_cash_balance=new_cash_balance, user_id=session["user_id"])
        return redirect(url_for("index"))
    return apology("Something is wrong here")

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "GET":
        return render_template("change_password.html")
    if request.method == "POST":
        # I am skipping through the step to check the password agin here
        new_password = request.form.get("new_password")
        db.execute("UPDATE users SET hash = :new_hash WHERE id = :user_id", new_hash=pwd_context.hash(new_password), user_id=session["user_id"])
        return redirect(url_for("login"))

