import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute("SELECT * FROM stocks WHERE user_id = ?", session['user_id'])
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])[0]['cash']
    user_stocks = db.execute("SELECT SUM(total) FROM stocks WHERE user_id = ?", session['user_id'])[0]['SUM(total)']
    if user_stocks is None:
        user_stocks = 0
    total = user_cash + user_stocks
    return render_template("index.html", stocks=stocks, user_cash=user_cash, user_stocks=user_stocks, total=total)


@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():
    """Allow users to change passwords"""
    if request.method == "POST":
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = ?", session['user_id'])

        # Ensure password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid password.", 403)

        new = request.form.get("confirmation")
        if new == '':
            return apology("Missing new password.", 403)

        # Update password to new password
        hash = generate_password_hash(new)
        db.execute("UPDATE users SET hash = ? WHERE id=?", hash, session['user_id'])

        return redirect("/")
    else:
        username = db.execute("SELECT username FROM users WHERE id = ?", session['user_id'])[0]['username']
        return render_template("profile.html", username=username)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Get stock's symbol
        symbol = request.form.get("symbol")
        # Validate symbol
        if symbol == '':
            return apology("Missing symbol.")
        info = lookup(symbol)
        if info is None:
            return apology("Symbol does not exist.")

        # Get number of shares and validate
        shares = request.form.get("shares")
        if not shares.isnumeric():
            return apology("Invalid number.")
        shares = int(shares)
        if shares < 1:
            return apology("Number of shares must be more than 1.")

        # Get stock's current price and get cash of user
        price = float(info["price"])
        total_price = price * shares
        cash = db.execute("SELECT cash FROM users WHERE id=?", session['user_id'])
        cash = float(cash[0]['cash'])

        # Check if user has enough cash to buy stocks
        if total_price > cash:
            return apology("Not enough cash to buy stocks.")

        # Update cash of user in db
        cash -= total_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session['user_id'])

        # Keep track of purchase in transactions table
        # CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, symbol TEXT NOT NULL, name TEXT NOT NULL, shares INTEGER, pp REAL NOT NULL, total REAL NOT NULL, datetime TEXT NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id));
        now = datetime.now()
        dt = now.strftime("%Y-%m-%d %H:%M:%S")
        name = info["name"]
        db.execute(
            "INSERT INTO transactions (user_id, symbol, name, shares, pp, total, datetime) VALUES (?, ?, ?, ?, ?, ?, ?)",
            session['user_id'], symbol, name, shares, price, total_price, dt)

        # Keep track of purchase in stocks table
        # CREATE TABLE stocks (user_id INTEGER, symbol TEXT NOT NULL, name TEXT NOT NULL, shares INTEGER, pp REAL NOT NULL, total REAL NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id));
        stocks = db.execute("SELECT * FROM stocks WHERE user_id = ? AND symbol = ?", session['user_id'], symbol)

        if not stocks:
            # User has no stocks, so INSERT user's stocks in db
            db.execute("INSERT INTO stocks (user_id, symbol, name, shares, pp, total) VALUES (?, ?, ?, ?, ?, ?)",
                       session['user_id'], symbol, name, shares, price, total_price)
            print(stocks)
        else:
            # User does have that stock, so UPDATE user's stocks in db
            current_shares = \
                db.execute("SELECT shares FROM stocks WHERE user_id=? AND symbol=?", session['user_id'], symbol)[0][
                    'shares']
            current_total_price = \
                db.execute("SELECT total FROM stocks WHERE user_id=? AND symbol=?", session['user_id'], symbol)[0][
                    'total']
            print(current_shares)
            print(current_total_price)
            db.execute("UPDATE stocks SET shares=?, total=? WHERE user_id=? AND symbol=?", current_shares + shares,
                       current_total_price + total_price, session['user_id'], symbol)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM transactions WHERE user_id = ?", session['user_id'])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if symbol == '':
            return apology("Missing symbol.")
        info = lookup(symbol)
        if info is None:
            return apology("Syumbol does not exist.")
        return render_template("quoted.html", info=info)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Get username / password
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check if username is blank or username already exists
        usernames = db.execute("SELECT username FROM users")
        if username == '':
            return apology("Missing username.")
        usernames = [i["username"] for i in usernames]

        if username in usernames:
            return apology("Username already exists")

        # Check if password is blank or passwords do not match
        if password == '':
            return apology("Missing password.")
        if password != confirmation:
            return apology("Invalid password")

        # INSERT new uster into users using a hash
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Get stock's symbol
        symbol = request.form.get("symbol")
        # Validate symbol
        if symbol is None:
            return apology("Missing symbol.")
        info = lookup(symbol)
        if info is None:
            return apology("Symbol does not exist.")

        # Render apology if user chooses stock that they do not own
        user_sym = db.execute("SELECT * FROM stocks WHERE user_id=? AND symbol=?", session['user_id'], symbol)
        if not user_sym:
            return apology("You do not own this stock.", 403)

        # Render apology if the input is not a positive integer or if the user does not own that many shares of the stock
        shares = int(request.form.get("shares"))
        if shares < 1:
            return apology("Number of shares must be more than 1.")

        # Get stock's current price and get cash of user
        price = float(info["price"])
        total_price = price * shares
        cash = db.execute("SELECT cash FROM users WHERE id=?", session['user_id'])
        cash = float(cash[0]['cash'])

        # Check if user has enough stocks to sell
        num_stocks = \
            db.execute("SELECT shares FROM stocks WHERE user_id=? AND symbol=?", session['user_id'], symbol)[0][
                'shares']
        if shares > num_stocks:
            return apology("You do not own that many stocks.")

        # Update cash of user in db
        cash += total_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session['user_id'])

        # Keep track of purchase in transactions table
        # CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, symbol TEXT NOT NULL, name TEXT NOT NULL, shares INTEGER, pp REAL NOT NULL, total REAL NOT NULL, datetime TEXT NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id));
        now = datetime.now()
        dt = now.strftime("%Y-%m-%d %H:%M:%S")
        name = info["name"]
        db.execute(
            "INSERT INTO transactions (user_id, symbol, name, shares, pp, total, datetime) VALUES (?, ?, ?, ?, ?, ?, ?)",
            session['user_id'], symbol, name, -shares, price, total_price, dt)

        # Keep track of purchase in stocks table

        # User does have that stock, so UPDATE user's stocks in db
        current_shares = \
            db.execute("SELECT shares FROM stocks WHERE user_id=? AND symbol=?", session['user_id'], symbol)[0][
                'shares']
        current_total_price = \
            db.execute("SELECT total FROM stocks WHERE user_id=? AND symbol=?", session['user_id'], symbol)[0]['total']

        db.execute("UPDATE stocks SET shares=?, total=? WHERE user_id=? AND symbol=?", current_shares - shares,
                   current_total_price - total_price, session['user_id'], symbol)

        # If user sold all of their stocks, delte row from table
        db.execute("DELETE FROM stocks WHERE shares=0")

        return redirect("/")
    else:
        symbols = db.execute("SELECT symbol FROM stocks WHERE user_id=?", session['user_id'])
        return render_template("sell.html", symbols=symbols)
