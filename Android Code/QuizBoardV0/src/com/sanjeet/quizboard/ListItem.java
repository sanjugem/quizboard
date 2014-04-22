/**
 * 
 */
package com.sanjeet.quizboard;

/**
 * @author developer
 *
 */
public class ListItem {
	
	
	
	
	public ListItem(int questionNumber, int answerNumber, boolean result,
			int time) {
		super();
		this.questionNumber = questionNumber;
		this.answerNumber = answerNumber;
		this.result = result;
		this.time = time;
	}
	
	public ListItem(int questionNumber, int answerNumber, char result,
			int time) {
		super();
		this.questionNumber = questionNumber;
		this.answerNumber = answerNumber;
		this.result = parseChar(result);
		this.time = time;
	}

	private int questionNumber;
	private int answerNumber;
	private boolean result;
	private int time;
	
	private boolean timeout;
	
	/**
	 * 
	 */
	public ListItem() {
		
	}

	/**
	 * @return the questionNumber
	 */
	public int getQuestionNumber() {
		return questionNumber;
	}

	/**
	 * @param questionNumber the questionNumber to set
	 */
	public void setQuestionNumber(int questionNumber) {
		this.questionNumber = questionNumber;
	}

	/**
	 * @return the answerNumber
	 */
	public int getAnswerNumber() {
		return answerNumber;
	}

	/**
	 * @param answerNumber the answerNumber to set
	 */
	public void setAnswerNumber(int answerNumber) {
		this.answerNumber = answerNumber;
	}

	/**
	 * @return the result
	 */
	public boolean isResult() {
		return result;
	}

	/**
	 * @param result the result to set
	 */
	public void setResult(boolean result) {
		this.result = result;
	}

	/**
	 * @return the time
	 */
	public int getTime() {
		return time;
	}

	/**
	 * @param time the time to set
	 */
	public void setTime(int time) {
		this.time = time;
	}
	
	private boolean parseChar(char c){
		this.timeout=false;
		if(c== 'c'){
			return true;
		}
		else if(c == 'f'){
			return false;
		}
		else if(c == 't'){
			this.timeout=true;
			return false;
		}
		return false;
	}

	/**
	 * @return the timeout
	 */
	public boolean isTimeout() {
		return timeout;
	}

	/**
	 * @param timeout the timeout to set
	 */
	public void setTimeout(boolean timeout) {
		this.timeout = timeout;
	}

}
