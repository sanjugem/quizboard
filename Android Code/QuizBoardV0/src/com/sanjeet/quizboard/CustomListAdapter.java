/**
 * 
 */
package com.sanjeet.quizboard;

import java.util.ArrayList;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

/**
 * @author developer
 *
 */
public class CustomListAdapter extends ArrayAdapter<ListItem> {

	
	Context context;
	ArrayList<ListItem> listItem;
	
	
	/**
	 * @param context
	 * @param textViewResourceId
	 * @param objects
	 */
	public CustomListAdapter(Context context, int textViewResourceId,
			ArrayList<ListItem> objects) {
		super(context, textViewResourceId, objects);
		this.context=context;
		this.listItem=objects;
	}

	
	
	
	@Override
	  public View getView(int position, View convertView, ViewGroup parent) {
	    LayoutInflater inflater = (LayoutInflater) context
	        .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	    View rowView = inflater.inflate(R.layout.listitem, parent, false);
	    TextView textView = (TextView) rowView.findViewById(R.id.itemText);
	    TextView textView2 = (TextView) rowView.findViewById(R.id.itemAns);
	    TextView textView3 = (TextView) rowView.findViewById(R.id.itemRes);
	    TextView textView4 = (TextView) rowView.findViewById(R.id.itemTime);
	    
	    textView.setText(this.listItem.get(position).getQuestionNumber()+"");
	    textView2.setText(this.listItem.get(position).getAnswerNumber()+"");
	    textView3.setText(this.listItem.get(position).isResult()+"");
	    textView4.setText(this.listItem.get(position).getTime()+"");
	   

	    return rowView;
	  }

}
